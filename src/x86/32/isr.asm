; SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
; SPDX-License-Identifier: GPL-3.0-or-later

global load_idt

%macro irq_handler 1
global irq%1
extern irq%1_handler
irq%1:
        pusha
        call irq%1_handler
        popa
        iret
%endmacro ; irq_handler

%macro generic_exception_handler 1
global exception%1
extern exception_handler
exception%1:
        pusha
        push %1
        call exception_handler
        popa
        add esp, 8
        sti
        iret
%endmacro ; generic_exception_handler

load_idt:
        mov edx, [esp + 4]
        lidt [edx]
        sti
        ret

generic_exception_handler 0
generic_exception_handler 1
generic_exception_handler 2
generic_exception_handler 3
generic_exception_handler 4
generic_exception_handler 5
generic_exception_handler 6
generic_exception_handler 7
generic_exception_handler 8
generic_exception_handler 9
generic_exception_handler 10
generic_exception_handler 11
generic_exception_handler 12
generic_exception_handler 13
generic_exception_handler 14
generic_exception_handler 15
generic_exception_handler 16
generic_exception_handler 17
generic_exception_handler 18
generic_exception_handler 19
generic_exception_handler 20
generic_exception_handler 21
generic_exception_handler 22
generic_exception_handler 23
generic_exception_handler 24
generic_exception_handler 25
generic_exception_handler 26
generic_exception_handler 27
generic_exception_handler 28
generic_exception_handler 29
generic_exception_handler 30
generic_exception_handler 31

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
