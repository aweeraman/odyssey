ENTRY(_start)

MEMORY
{
    ram : ORIGIN = 0x82000000, LENGTH = 256K
}
 
SECTIONS
{
        kernel_begin = .;
        .text :
        {
                *(.boot)
                *(.text)
        } > ram
        .rodata :
        {
                *(.rodata)
        } > ram
        .data :
        {
                *(.data)
        } > ram
        .bss :
        {
                *(.bss)
        } > ram
        kernel_end = .;
}
