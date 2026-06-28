ENTRY(_start) SECTIONS
{
    . = 1M;   
    .text : { 
        *(.multiboot)
        *(.text) 
        *(.rodata)
    }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
