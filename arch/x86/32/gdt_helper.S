/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#define KERNEL_CODE_SEGMENT $0x08
#define KERNEL_DATA_SEGMENT $0x10
#define TASK_STATE_SEGMENT  $0x28

        .global load_gdt
	.global flush_tss
	.global user_mode_init

load_gdt:
        movl 4(%esp), %eax
        lgdt (%eax)

        movw KERNEL_DATA_SEGMENT, %ax
        movw %ax, %ds
        movw %ax, %es
        movw %ax, %fs
        movw %ax, %gs
        movw %ax, %ss

        jmp KERNEL_CODE_SEGMENT, $reload

flush_tss:
	movw TASK_STATE_SEGMENT, %ax
	ltr %ax
	ret

user_mode_init:
	movw $0x2B, %ax	# user data segment, bottom 2 bits set for user mode
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	movl %eax, %esp
	pushw 0x2B
	pushl %eax
	pushf
	pushw 0x1B
	pushl user_function
	iret

reload:
        ret
