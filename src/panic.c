#include "io.h"
#include "utils.h"

__attribute__((noreturn)) void kernel_panic(const char *msg, const char *file, unsigned int line) {
    __asm__ __volatile__("cli");

    print_string("\n==================================================\n");
    print_string("!!!!!!!!!!!!!!!   KERNEL PANIC   !!!!!!!!!!!!!!!\n");
    print_string("==================================================\n");
    
    print_string("REASON: ");
    print_string(msg);
    print_string("\n\n");
    
    print_string("LOCATION: ");
    print_string(file);
    print_string(":");
    print_int(line);
    print_string("\n");
    
    print_string("==================================================\n");
    print_string("System execution safely halted. Please reset CPU.\n");

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
