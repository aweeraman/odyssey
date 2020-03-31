  .set STACK_SIZE,    4096
  .set MB2_MAGIC,     0xe85250d6
  .set MB2_ARCH_FLAG, 0x0
  .set MB2_FB_TAG,    0x0

  .global _start

  .section .text
mb2_hdr:
  .align 4
  .long MB2_MAGIC             # multiboot2 magic string
  .long MB2_ARCH_FLAG         # architecture
  .long mb2_hdr_end - mb2_hdr # header length
  .long -(MB2_MAGIC + MB2_ARCH_FLAG + (mb2_hdr_end - mb2_hdr)) # checksum
  .word 0x0                   # end tag type
  .word 0x0                   # end tag flags
  .long 0x8                   # size, 8 to terminate
mb2_hdr_end:

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
