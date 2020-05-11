ENTRY(_start)
 
SECTIONS
{
        . = 1M;
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
