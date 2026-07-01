#include "io.h"
#include "shell.h"
#include "tarfs.h"
#include "panic.h"

__attribute__((noreturn)) void __stack_chk_fail(void) {
    print_string("KERNEL PANIC: Stack Smashing Detected!");
    while(1) {
        __asm__ __volatile__("cli; hlt");
    }
}

void __stack_chk_fail_local(void) {
    __stack_chk_fail();
}

int start_kernel(unsigned int mb_address) {
    // Init fs
    print_string("Initializing init_tarfs...\n");
    int ret = init_tarfs((void*)mb_address);
    if (ret != 0) {
        PANIC("Failed to initialize init_tarfs.");
    }

    // Setup shell
    print_string("Hello from Crei!\n");
    print_string("$ ");
    line_start_position = cursor_position;

    while(1) {
        run_shell();
    }

    return 0;
}
