#pragma once

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);

typedef struct {
    unsigned char scancode;
    char ascii;
} IBMPair;

extern IBMPair global_ibm_array[];
