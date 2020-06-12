ENTRY(_start)
 
SECTIONS
{
	. = 1M;
	kernel_begin = .;
	.text ALIGN(0x1000):
	{
		*(.multiboot)
		*(.text)
	}
	.rodata ALIGN(0x1000):
	{
		*(.rodata)
	}
	.data ALIGN(0x1000):
	{
		*(.data)
	}
	.bss ALIGN(0x1000):
	{
		*(.bss)
	}
	kernel_end = .;
}
