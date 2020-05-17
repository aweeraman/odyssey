/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

        .global load_idt

.macro irq_handler num
        .global irq\num
irq\num\() :
        pushal
        call irq\num\()_handler
        popal
        iret
.endm

.macro exception_handler num
        .global exception\num
exception\num\() :
        pushal
        pushl $\num
        call exception_handler
        popal
        addl $8, %esp
        sti
        iret
.endm

load_idt:
        movl 4(%esp), %edx
        lidt (%edx)
        sti
        ret

exception_handler 0
exception_handler 1
exception_handler 2
exception_handler 3
exception_handler 4
exception_handler 5
exception_handler 6
exception_handler 7
exception_handler 8
exception_handler 9
exception_handler 10
exception_handler 11
exception_handler 12
exception_handler 13
exception_handler 14
exception_handler 15
exception_handler 16
exception_handler 17
exception_handler 18
exception_handler 19
exception_handler 20
exception_handler 21
exception_handler 22
exception_handler 23
exception_handler 24
exception_handler 25
exception_handler 26
exception_handler 27
exception_handler 28
exception_handler 29
exception_handler 30
exception_handler 31

irq_handler 0
irq_handler 1
irq_handler 2
irq_handler 3
irq_handler 4
irq_handler 5
irq_handler 6
irq_handler 7
irq_handler 8
irq_handler 9
irq_handler 10
irq_handler 11
irq_handler 12
irq_handler 13
irq_handler 14
irq_handler 15
