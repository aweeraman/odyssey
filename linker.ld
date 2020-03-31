ENTRY(_start)
 
SECTIONS
{
	. = 1M;
	.text   ALIGN(4K): { *(.text) }
	.rodata ALIGN(4K): { *(.rodata) }
	.data   ALIGN(4K): { *(.data) }
	.bss    ALIGN(4K): { *(.bss) }
}
