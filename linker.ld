ENTRY(_start)
 
SECTIONS
{
	. = 1M;
	.boot   ALIGN(8):
  {
    *(.multiboot)
  }
	.text   ALIGN(8):
  {
    *(.text)
  }
	.rodata ALIGN(8):
  {
    *(.rodata)
  }
	.data   ALIGN(8):
  {
    *(.data)
  }
	.bss    ALIGN(8):
  {
    *(.bss)
  }
}
