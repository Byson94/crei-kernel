bits 32
section .multiboot
    dd 0x1BADB002   ; Satisfy Grub
    dd 0x00         ; Flags (0 means no special requests)
    dd -0x1BADB002  ; Checksum (Magic + Flags + Checksum must = 0)

section .text
global _start
extern start_kernel
_start:
    mov esp, stack_top

    push ebx
    push eax

    jmp start_kernel

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
