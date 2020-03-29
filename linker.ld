ENTRY(_start)
 
SECTIONS
{
	. = 1M;
	.text   : { *(.text) }
	.rodata : { *(.rodata) }
	.data   : { *(.data) }
	.bss    : { *(.bss) }
}
