#include "io.h"
#include "shell.h"
#include "utils.h"
#include "panic.h"
#include "hardware.h"

static const char* helps[5][2] = {
    { "echo", "Echo something" },
    { "clear", "Clear Buffer" },
    { "shutdown", "Attempt to shutdown this device" },
    { "kmsg", "Send a message to the kernel" },
    { "help", "Print this help" }
};

void print_padded_string(const char *str, unsigned int width) {
    print_string(str);
    
    unsigned int len = string_length(str);
    if (len < width) {
        unsigned int spaces_to_print = width - len;
        for (unsigned int i = 0; i < spaces_to_print; i++) {
            print_string(" ");
        }
    }
}

// == Shell basic ==
void shell_help() {
    int help_size = sizeof(helps)/sizeof(helps[0]);

    // Find longest command
    unsigned int max_cmd_len = 0;
    for (int i = 0; i < help_size; i++) {
        unsigned int current_len = string_length(helps[i][0]);
        if (current_len > max_cmd_len) {
            max_cmd_len = current_len;
        }
    }

    unsigned int dynamic_width = max_cmd_len + 2;

    print_string("Kesh - Kernel Embeded SHell\n\n");
    print_string("Basic Commands: \n");

    for (int i = 0; i < help_size; i++) {
        print_string("  ");
        print_padded_string(helps[i][0], dynamic_width);
        print_string(" - ");
        print_string(helps[i][1]);
        print_string("\n");
    }
}

void shell_clear() {
    volatile char *video_memory = (volatile char*) 0xB8000;

    for (unsigned int i = 0; i <= cursor_position; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = 0x0F;
    }

    cursor_position = 0;
    line_start_position = 0;
}

void shell_shutdown() {
    __asm__ __volatile__("outw %1, %0" : : "dN"((unsigned short)0x604), "a"((unsigned short)0x2000));
    __asm__ __volatile__("outw %1, %0" : : "dN"((unsigned short)0xB004), "a"((unsigned short)0x2000));
    __asm__ __volatile__("outl %1, %0" : : "dN"((unsigned short)0x4004), "a"((unsigned int)0x3400));

    print_string("Emulator bypass failed. Attempting APM hardware power-off...\n");

    __asm__ __volatile__ (
        "mov $0x5307, %%ax\n\t"
        "mov $0x0001, %%bx\n\t"
        "mov $0x0003, %%cx\n\t"
        "int $0x15"
        : : : "ax", "bx", "cx"
    );

    print_string("\nShutdown failed. ACPI support required for this motherboard.\n");
    print_string("You can now safely press the physical power button.\n");

    while (1) {
        __asm__ __volatile__("cli; hlt");
    }
}

void handle_command(int argc, char** argv) {
    if (strcmp(argv[0], "help") == 0) {
        shell_help();
    } else if (strcmp(argv[0], "echo") == 0) {
        for (int i = 1; i <= argc - 1; i++) {
            print_string(argv[i]);
            print_string(" ");
        }
        print_string("\n");
    } else if (strcmp(argv[0], "clear") == 0) {
        shell_clear();
    } else if (strcmp(argv[0], "shutdown") == 0) {
        shell_shutdown();
    } else if (strcmp(argv[0], "kmsg") == 0) {
        if (argc < 2) {
            print_string("Pass a message to evaluate.\n");
            return;   
        }

        if (strcmp(argv[1], "k22halt") == 0) {
            print_string("Halting under user request...\n");
            print_string("You can press the physical power button to shutdown.\n");
            __asm__ __volatile__("cli; hlt");
        } else if (strcmp(argv[1], "k22panic") == 0) {
            PANIC("User requested panic!");
        } else {
            print_string("Unknown message:");
            print_string(argv[1]);
            print_string("\n");
        }
    } else {
        print_string("Command not found: ");
        print_string(argv[0]);
        print_string("\n");
    }
}

void run_shell() {
    unsigned char status = inb(0x64);

    if (status & 0x01) {
        unsigned char scancode = inb(0x60);
        
        if (scancode < 0x80) {
            int i = 0;
            while (global_ibm_array[i].scancode != 0x0) {
                if (global_ibm_array[i].scancode == scancode) {
                    char* string = (char*)global_ibm_array[i].ascii;
                    int cursor_before_print = cursor_position;

                    print_string(string);
                    volatile char *video_memory = (volatile char*) 0xB8000;

                    // handle newlines (enter)
                    if (string[0] == '\n') {
                        // 81 is the length of each line
                        // We assume the command is within 81 chars.
                        char cmd[81];
                        int cmd_len = cursor_before_print - line_start_position;

                        if (cmd_len > 80) cmd_len = 80;
                        if (cmd_len < 0)  cmd_len = 0;

                        int i = 0;
                        for (i = 0; i < cmd_len; i++) {
                            int screen_index = line_start_position + i;
                            cmd[i] = video_memory[screen_index * 2];
                        }
                        cmd[i] = '\0';

                        char* argv[81];
                        size_t count = split_whitespace_inplace(cmd, 81, argv);
                        handle_command(count, argv);
                
                        // setep shell
                        print_string("$ ");
                        line_start_position = cursor_position;
                    } 

                    break;
                }
                i++;
            }
        }
    }
}
