#include "io.h"
#include "hardware.h"

unsigned int cursor_position = 0;
unsigned int line_start_position = 0;

unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void outb(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %%al, %%dx" : : "a"(data), "d"(port));
}

IBMPair global_ibm_array[] = {
    {0x02, "1"},  {0x03, "2"},  {0x04, "3"},  {0x05, "4"},
    {0x06, "5"},  {0x07, "6"},  {0x08, "7"},  {0x09, "8"},
    {0x0A, "9"},  {0x0B, "0"},  {0x0C, "-"},  {0x0D, "="},
    {0x0E, "\b"}, // Backspace
    {0x0F, "\t"}, // Tab
    {0x10, "q"},  {0x11, "w"},  {0x12, "e"},  {0x13, "r"},
    {0x14, "t"},  {0x15, "y"},  {0x16, "u"},  {0x17, "i"},
    {0x18, "o"},  {0x19, "p"},  {0x1A, "["},  {0x1B, "]"},
    {0x1C, "\n"}, // Enter
    {0x1E, "a"},  {0x1F, "s"},  {0x20, "d"},  {0x21, "f"},
    {0x22, "g"},  {0x23, "h"},  {0x24, "j"},  {0x25, "k"},
    {0x26, "l"},  {0x27, ";"},  {0x28, "'"},  {0x29, "`"},
    {0x2B, "\\"}, {0x2C, "z"},  {0x2D, "x"},  {0x2E, "c"},
    {0x2F, "v"},  {0x30, "b"},  {0x31, "n"},  {0x32, "m"},
    {0x33, ","},  {0x34, "."},  {0x35, "/"},  {0x39, " "},
    {0x0, 0}
};

void print_string(char *string_ptr) {
    char *str = string_ptr;
    volatile char *video_memory = (volatile char*) 0xB8000;
    
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            cursor_position += (80 - (cursor_position % 80));
        } else if (str[i] == '\b') {
            if (cursor_position > line_start_position) {
                cursor_position--;
                video_memory[cursor_position * 2] = ' ';
                video_memory[cursor_position * 2 + 1] = 0x0F;
            }
        } else if (str[i] == '\t') {
            cursor_position += (4 - (cursor_position % 4));
        } else {
            video_memory[cursor_position * 2] = str[i];
            video_memory[cursor_position * 2 + 1] = 0x0F;
            cursor_position++;
        }

        i++;
    }
    update_hardware_cursor(cursor_position);
}
