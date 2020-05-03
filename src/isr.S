        .global load_idt

load_idt:
        movl 4(%esp), %edx
        lidt (%edx)
        sti
        ret

.macro irq_handler num
        .global irq\num

irq\num\() :
        pusha
        call irq\num\()_handler
        popa
        iret
.endm

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
