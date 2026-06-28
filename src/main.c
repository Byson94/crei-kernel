#include "io.h"
#include "shell.h"

int main() {
    print_string("Hello from Crei!\n");
    print_string("$ ");
    line_start_position = cursor_position;

    while(1) {
        run_shell();
    }

    return 0;
}
