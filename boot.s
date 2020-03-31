.set STACK_SIZE, 4096

.global _start

.section .text
multiboot_start:
  .align 4
  .long 0xe85250d6
  .long 0x0
  .long multiboot_end - multiboot_start
  .long 0x100000000 - (0xe85250d6 + 0 + (multiboot_end - multiboot_start))
  .long 0x0
  .long 0x0
  .long 0x8
multiboot_end:

_start:
  mov $stack_top, %esp
  call kernel_main
  cli
  hlt

.section .bss
  .align 4
stack_bottom:
  .skip STACK_SIZE
stack_top:
