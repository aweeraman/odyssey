/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2020, Anuradha Weeraman
 */

#define ASM_FILE 1
#include "multiboot2.h"

  .set STACK_SIZE,     CONFIG_STACK
  .set MB2_ARCH_FLAG,  0x0
  .set MB2_FB_TAG,     0x0

  .section .multiboot
  .align 8
mb2_hdr:
  .long MULTIBOOT2_HEADER_MAGIC
  .long MB2_ARCH_FLAG
  .long mb2_hdr_end - mb2_hdr
  .long -(MULTIBOOT2_HEADER_MAGIC + MB2_ARCH_FLAG + (mb2_hdr_end - mb2_hdr))
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

  # Clear EFLAGS
  pushl $0
  popf

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
