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
  .word 0x0                   # end tag type
  .word 0x0                   # end tag flags
  .long 0x8                   # size, 8 to terminate
mb2_hdr_end:

  .section .text
  .global _start
_start:
   mov $stack_top, %esp
   call kernel_main
   cli
1: hlt
   jmp 1b

  .section .bss
  .align 4
stack_bottom:
  .skip STACK_SIZE
stack_top:
