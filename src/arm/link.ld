ENTRY(_start)
 
SECTIONS
{
        . = 0x80000000;
        kernel_begin = .;
        .text :
        {
                *(.boot)
                *(.text)
        }
        .rodata :
        {
                *(.rodata)
        }
        .data :
        {
                *(.data)
        }
        .bss :
        {
                *(.bss)
        }
        kernel_end = .;
}
