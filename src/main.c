#include "io.h"

int main() {
    print_string("Hello from Crei!\n");
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
                        print_string((char*)global_ibm_array[i].ascii);
                        break;
                    }
                    i++;
                }
            }
        }
    }

    return 0;
}
