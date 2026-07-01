#pragma once

#include <stddef.h>

// 2MB
#define TARFS_MAX_SIZE 0x200000

int init_tarfs(void* multiboot_structure);
char** tarfs_list(char* tarfs_addr);
void* tarfs_mkdir(char* tarfs_addr, size_t max_size, const char* name);
void* tarfs_mkfile(char* tarfs_addr, size_t max_size, const char* name);

extern void* tarfs_root;
