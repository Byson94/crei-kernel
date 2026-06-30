bits 32
section .multiboot
    dd 0x1BADB002   ; Satisfy Grub
    dd 0x00         ; Flags (0 means no special requests)
    dd -0x1BADB002  ; Checksum (Magic + Flags + Checksum must = 0)

section .text
global _start
extern start_kernel
_start:
    jmp start_kernel
