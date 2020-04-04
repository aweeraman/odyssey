  .set STACK_SIZE,    4096
  .set MB2_MAGIC,     0xe85250d6
  .set MB2_ARCH_FLAG, 0x0
  .set MB2_FB_TAG,    0x0

  .section .multiboot
  .align 8
mb2_hdr:
  .long MB2_MAGIC             # multiboot2 magic string
  .long MB2_ARCH_FLAG         # architecture
  .long mb2_hdr_end - mb2_hdr # header length
  .long -(MB2_MAGIC + MB2_ARCH_FLAG + (mb2_hdr_end - mb2_hdr)) # checksum
  .align 8
tag_end:
  .word 0                   # end tag type
  .word 0                   # end tag flags
  .long 8                   # size, 8 to terminate
tag_end_end:
mb2_hdr_end:

  .section .text
  .global _start
_start:
  mov $stack_top, %esp

  # Pointer to the Multiboot2 information structure
  pushl %ebx

  # Multiboot2 magic value
  pushl %eax

  # Jump to function in kernel.c
  call kernel_main

  # returning from the kernel
  pushl $shutdown_msg
  call print

  cli
1: hlt
  jmp 1b

shutdown_msg:
  .asciz  "System shutdown.\n"

  .section .bss
  .align 16
stack_bottom:
  .skip STACK_SIZE
stack_top:
