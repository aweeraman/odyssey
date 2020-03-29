.global _start

.section .text
  .align 4
  .long 0x1BADB002
  .long 0x0
  .long -0x1BADB002
  .type _start, @function

  _start:
	mov $stack_top, %esp
	call kernel_main
	cli
  hlt

.section .bss
  stack_bottom:
  .skip 2048
  stack_top:
