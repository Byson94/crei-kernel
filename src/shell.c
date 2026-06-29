#include "io.h"
#include "shell.h"
#include "hardware.h"

void handle_command(char* cmd) {
    if (io_strcmp(cmd, "help") == 0) {
        print_string("Help will arrive in two business days.\n");
    } else {
        print_string("Command not found: '");
        print_string(cmd);
        print_string("'\n");
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

                        handle_command(cmd);
                
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
