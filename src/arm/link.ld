ENTRY(_start)
 
MEMORY
{
    ram : ORIGIN = 0x80000000, LENGTH = 0x10000
}

SECTIONS
{
        kernel_begin = .;
        .text :
        {
                *(.boot)
                *(.text)
        }> ram
        .rodata :
        {
                *(.rodata)
        }> ram
        .data :
        {
                *(.data)
        }> ram
        .bss :
        {
                *(.bss)
        }> ram
        kernel_end = .;
}
