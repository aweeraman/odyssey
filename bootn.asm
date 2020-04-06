global _start

extern kernel_main
extern print

MB2_MAGIC     equ 0xe85250d6
MB2_ARCH_FLAG equ 0x0
STACK_SIZE    equ 4096

section .multiboot
align 8
mb2_hdr:
  dd MB2_MAGIC
  dd MB2_ARCH_FLAG
  dd mb2_hdr_end - mb2_hdr
  dd -(MB2_MAGIC + MB2_ARCH_FLAG + (mb2_hdr_end - mb2_hdr))
tag_end:
  dw 0
  dw 0
  dw 8
tag_end_end:
mb2_hdr_end:

section .text
_start:
  mov esp, stack_top

  ; Pointer to the Multiboot2 information structure
  push ebx

  ; Multiboot2 magic value
  push eax

  ; Jump to function in kernel.c
  call kernel_main

  ; returning from the kernel
  push message
  call print

  cli
  hlt

message db "System shutdown.", 10, 0

section .bss
align 32
stack_bottom:
  resb STACK_SIZE
stack_top:
