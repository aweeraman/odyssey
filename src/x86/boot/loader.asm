; SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
; SPDX-License-Identifier: GPL-3.0-or-later

global _start

extern kernel_main
extern early_framebuffer_console_init;
extern gdt_init
extern pic_init
extern idt_init
extern printk
extern cpuid

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

        ; Initialize the console
        call early_framebuffer_console_init;

        ; Check for the availability of the cpuid instruction
        pushfd
        pushfd
        xor dword [esp], 0x00200000
        popfd
        pushfd
        pop eax
        xor eax, [esp]
        popfd
        and eax, 0x00200000
        cmp eax, 0
        je _nocpuid
        call cpuid

_nocpuid:
        ; Disable interrupts
        cli

        ; Setup the GDT
        call gdt_init

        ; Initialize the PIC
        call pic_init

        ; Setup the IDT
        call idt_init

        ; Enter protected mode
        mov eax, cr0
        or eax, 1
        mov cr0, eax

        ; Jump to function in kernel.c
        call kernel_main

        ; returning from the kernel
        push message
        call printk

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
