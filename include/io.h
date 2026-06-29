#pragma once

extern unsigned int cursor_position;
extern unsigned int line_start_position;

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
void print_string(char *str);
int  io_strcmp(const char *str1, const char *str2);

typedef struct {
    unsigned char scancode;
    const char *ascii;
} IBMPair;

extern IBMPair global_ibm_array[];
