#include "io.h"
#include "shell.h"

void run_shell() {
    unsigned char status = inb(0x64);

    if (status & 0x01) {
        unsigned char scancode = inb(0x60);
        
        if (scancode < 0x80) {
            int i = 0;
            while (global_ibm_array[i].scancode != 0x0) {
                if (global_ibm_array[i].scancode == scancode) {
                    char* string = (char*)global_ibm_array[i].ascii;
                    print_string(string);

                    // handle newlines (enter)
                    if (global_ibm_array[i].ascii[0] == '\n') {
                        // handle cmd
                        volatile char *video_memory = (volatile char*) 0xB8000;

                        char cmd[81];
                        int cmd_len = cursor_position - line_start_position;

                        if (cmd_len > 80) cmd_len = 80;
                        if (cmd_len < 0)  cmd_len = 0;

                        int i = 0;
                        for (i = 0; i <= cmd_len; i++) {
                            int screen_index = line_start_position + i;
                            cmd[i] = video_memory[screen_index * 2];
                        }
                        cmd[i] = '\0';
                
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
