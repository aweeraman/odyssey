;**
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <https://www.gnu.org/licenses/>.
;
; Copyright 2020, Anuradha Weeraman
;

global _start

extern kernel_main
extern printf

MB2_MAGIC                equ 0xe85250d6
MB2_ARCH_FLAG            equ 0x0
MB2_HEADER_TAG_FB        equ 0x5
MB2_HEADER_TAG_OPTIONAL  equ 0x1
STACK_SIZE               equ 4096

section .multiboot
align 8
mb2_hdr:
  dd MB2_MAGIC
  dd MB2_ARCH_FLAG
  dd mb2_hdr_end - mb2_hdr
  dd -(MB2_MAGIC + MB2_ARCH_FLAG + (mb2_hdr_end - mb2_hdr))
%ifdef CONSOLE_GRAPHICS
framebuffer_tag_start:
  dw MB2_HEADER_TAG_FB
  dw MB2_HEADER_TAG_OPTIONAL
  dd framebuffer_tag_end - framebuffer_tag_start
  dd FB_WIDTH
  dd FB_HEIGHT
  dd FB_BPP
framebuffer_tag_end:
%endif
  align 8
tag_end:
  dw 0
  dw 0
  dw 8
tag_end_end:
mb2_hdr_end:

section .text
_start:
  mov esp, stack_top

  ; Clear EFLAGS
  push 0
  popf

  ; Pointer to the Multiboot2 information structure
  push ebx

  ; Multiboot2 magic value
  push eax

  ; Jump to function in kernel.c
  call kernel_main

  ; returning from the kernel
  push message
  call printf

  cli
  hlt

message db "System shutdown.", 10, 0

section .bss
align 32
stack_bottom:
  resb STACK_SIZE
stack_top:
