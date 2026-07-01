#include "tarfs.h"
#include "utils.h"
#include <stddef.h>
#include <stdint.h>

__attribute__((aligned(4096))) static char tarfs_writable_buffer[TARFS_MAX_SIZE];
void* tarfs_root = NULL;

int init_tarfs(void* multiboot_structure) {
    unsigned int* mb_array = (unsigned int*)multiboot_structure;
    unsigned int mods_count = mb_array[5];

    if (mods_count == 0) {
        return -1; // No modules loaded
    }

    unsigned int raw_mods_ptr = mb_array[6];
    unsigned int* mods_list = (unsigned int*)raw_mods_ptr;

    // Multiboot module structure offsets:
    // mods_list[0] = mod_start (physical address)
    // mods_list[1] = mod_end (physical address)
    uint32_t mod_start = mods_list[0];
    uint32_t mod_end   = mods_list[1];
    uint32_t mod_size  = mod_end - mod_start;

    if (mod_size > TARFS_MAX_SIZE) {
        return -2; // RAM disk is too big for our buffer!
    }

    char* src = (char*)(uintptr_t)mod_start;
    for (uint32_t i = 0; i < mod_size; i++) {
        tarfs_writable_buffer[i] = src[i];
    }

    tarfs_root = (void*)tarfs_writable_buffer;

    return 0;
}

unsigned int parse_octal_size(char* size_ptr) {
    unsigned int size = 0;
    
    for (int i = 0; i < 11; i++) {
        if (size_ptr[i] == ' ' || size_ptr[i] == '\0') {
            break;
        }
        size = (size * 8) + (size_ptr[i] - '0');
    }
    return size;
}

static char* file_list[32]; 

char** tarfs_list(char* tarfs_addr) {
    char* current_block = tarfs_addr;
    int file_count = 0;

    while (current_block[0] != '\0') {
        
        file_list[file_count] = current_block; 
        file_count++;

        if (file_count >= 32) break;

        unsigned int file_size = parse_octal_size(&current_block[124]);

        unsigned int aligned_size = (file_size + 511) & ~511;

        current_block += 512 + aligned_size;
    }

    file_list[file_count] = 0; 

    return file_list;
}

void* tarfs_mkdir(char* tarfs_addr, size_t max_size, const char* name) {
    char* current_block = tarfs_addr;
    char* end_bound = tarfs_addr + max_size;

    while (current_block + 512 <= end_bound && current_block[0] != '\0') {
        unsigned int file_size = parse_octal_size(&current_block[124]);
        unsigned int aligned_size = (file_size + 511) & ~511;
        
        current_block += 512 + aligned_size;
    }

    if (current_block + 1024 > end_bound) {
        return NULL; 
    }

    volatile char* v_block = (volatile char*)current_block;

    int i = 0;
    while (name[i] != '\0' && i < 99) {
        v_block[i] = name[i];
        i++;
    }

    if (i > 0 && v_block[i-1] != '/') {
        v_block[i] = '/';
        i++;
    }
    
    for (int n = i; n < 100; n++) {
        __asm__ __volatile__("" : : : "memory");
        v_block[n] = '\0';
    }

    for (int j = 0; j < 11; j++) {
        __asm__ __volatile__("" : : : "memory");
        v_block[124 + j] = '0';
    }
    v_block[124 + 11] = '\0';

    v_block[156] = '5';

    for (int m = 157; m < 512; m++) {
        __asm__ __volatile__("" : : : "memory");
        v_block[m] = '\0';
    }
    
    for (int c = 0; c < 8; c++) {
        v_block[148 + c] = ' ';
    }

    volatile char* next_null_block = (volatile char*)(current_block + 512);
    for (int k = 0; k < 512; k++) {
        __asm__ __volatile__("" : : : "memory");
        next_null_block[k] = '\0';
    }

    return (void*)current_block;
}

void* tarfs_mkfile(char* tarfs_addr, size_t max_size, const char* name) {
    char* current_block = tarfs_addr;
    char* end_bound = tarfs_addr + max_size;

    while (current_block + 512 <= end_bound && current_block[0] != '\0') {
        unsigned int file_size = parse_octal_size(&current_block[124]);
        unsigned int aligned_size = (file_size + 511) & ~511;
        
        char* next_block = current_block + 512 + aligned_size;
        if (next_block <= current_block || next_block > end_bound) {
            return NULL;
        }
        current_block = next_block;
    }

    if (current_block + 1536 > end_bound) {
        return NULL; 
    }

    volatile char* v_block = (volatile char*)current_block;

    int i = 0;
    while (name[i] != '\0' && i < 99) {
        v_block[i] = name[i];
        i++;
    }

    for (int n = i; n < 100; n++) {
        __asm__ __volatile__("" : : : "memory");
        v_block[n] = '\0';
    }

    for (int j = 0; j < 11; j++) {
        __asm__ __volatile__("" : : : "memory");
        v_block[124 + j] = '0';
    }
    v_block[124 + 11] = '\0';

    v_block[156] = '0';

    for (int m = 157; m < 512; m++) {
        __asm__ __volatile__("" : : : "memory");
        v_block[m] = '\0';
    }
    
    for (int c = 0; c < 8; c++) {
        v_block[148 + c] = ' ';
    }

    volatile char* data_block = (volatile char*)(current_block + 512);
    for (int k = 0; k < 512; k++) {
        __asm__ __volatile__("" : : : "memory");
        data_block[k] = '\0'; 
    }

    volatile char* eof_block = (volatile char*)(current_block + 1024);
    for (int k = 0; k < 512; k++) {
        __asm__ __volatile__("" : : : "memory");
        eof_block[k] = '\0';
    }

    return (void*)current_block;
}

static int tarfs_name_match(const char* tar_name, const char* search_name) {
    size_t i = 0;
    while (i < 100) {
        if (search_name[i] == '\0') {
            // Exact match
            if (tar_name[i] == '\0') return 1;
            // Match if the TAR entry is a directory version of our search (has a trailing slash)
            if (tar_name[i] == '/' && tar_name[i + 1] == '\0') return 1;
            return 0;
        }
        if (tar_name[i] != search_name[i]) {
            return 0;
        }
        i++;
    }
    return 0;
}

void* tarfs_find(char* tarfs_addr, const char* name) {
    char* current_block = tarfs_addr;

    while (current_block[0] != '\0') {
        
        if (tarfs_name_match(current_block, name)) {
            return (void*)current_block; 
        }

        unsigned int file_size = parse_octal_size(&current_block[124]);
        unsigned int aligned_size = (file_size + 511) & ~511;
        
        current_block += 512 + aligned_size;
    }

    return NULL;
}
