#include "io.h"

unsigned int cursor_position = 0;

void print_string(char *string_ptr) {
    char *str = string_ptr;
    
    volatile char *video_memory = (volatile char*) 0xB8000;
    
    int i = 0;
    while (str[i] != '\0') {
        video_memory[cursor_position * 2] = str[i];
        video_memory[cursor_position * 2 + 1] = 0x0F;
        
        cursor_position++;
        i++;
    }
}

int main() {
    print_string("Hello from Crei!");
    cursor_position++;

    while(1) {
        unsigned char status = inb(0x64);

        if (status & 0x01) {
            unsigned char scancode = inb(0x60);
            
            if (scancode < 0x80) {
                volatile char *video_memory = (volatile char*) 0xB8000;
                int i = 0;
                
                while (global_ibm_array[i].scancode != 0x0) {
                    if (global_ibm_array[i].scancode == scancode) {
                        video_memory[cursor_position * 2] = global_ibm_array[i].ascii;
                        video_memory[cursor_position * 2 + 1] = 0x02; 
                        
                        cursor_position++;
                        break;
                    }
                    i++;
                }
            }
        }
    }

    return 0;
}
