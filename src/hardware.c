#include "io.h"
#include "hardware.h"

void update_hardware_cursor(unsigned int position) {
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
}

