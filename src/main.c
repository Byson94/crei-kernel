#include "io.h"
#include "shell.h"

__attribute__((noreturn)) void __stack_chk_fail(void) {
    print_string("KERNEL PANIC: Stack Smashing Detected!");
    while(1) {
        __asm__ __volatile__("cli; hlt");
    }
}

void __stack_chk_fail_local(void) {
    __stack_chk_fail();
}

int main() {
    // Setup shell
    print_string("Hello from Crei!\n");
    print_string("$ ");
    line_start_position = cursor_position;

    while(1) {
        run_shell();
    }

    return 0;
}
