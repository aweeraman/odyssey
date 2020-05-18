; SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
; SPDX-License-Identifier: GPL-3.0-or-later
;

global _start

extern kernel_main
extern gdt_init
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

%ifdef CONFIG_FRAMEBUFFER_RGB
framebuffer_tag_start:
        dw MB2_HEADER_TAG_FB
        dw MB2_HEADER_TAG_OPTIONAL
        dd framebuffer_tag_end - framebuffer_tag_start
        dd CONFIG_FRAMEBUFFER_WIDTH
        dd CONFIG_FRAMEBUFFER_HEIGHT
        dd CONFIG_FRAMEBUFFER_BPP
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

        ; Disable interrupts
        cli

        ; Setup the GDT
        call gdt_init

        ; Enter protected mode
        mov eax, cr0
        or eax, 1
        mov cr0, eax

        ; Jump to function in kernel.c
        call kernel_main

        ; returning from the kernel
        push message
        call printf

        cli
_idle:
        hlt
        jmp _idle

message db "System shutdown.", 10, 0

section .bss
align 32
stack_bottom:
        resb STACK_SIZE
stack_top:
