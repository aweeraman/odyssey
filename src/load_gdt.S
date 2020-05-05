/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#define KERNEL_CODE_SEGMENT $0x08
#define KERNEL_DATA_SEGMENT $0x10

        .global load_gdt

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

reload:
        ret
