/**
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright 2020, Anuradha Weeraman
 */

#include "pic.h"
#include "io.h"
#include "libk.h"

// IRQ greater than 7 EOI must be sent to both master and slave
void pic_eoi(int irq)
{
        if (irq > 7)
                outb(PIC_SLAVE_COMMAND, EOI);

        outb(PIC_MASTER_COMMAND, EOI);
}

void pic_init()
{
        printf("Initializing PIC\n");
	outb(PIC_MASTER,      0x11);
        outb(PIC_SLAVE,       0x11);
        outb(PIC_MASTER_DATA, 0x20);
        outb(PIC_SLAVE_DATA,  0x28);
        outb(PIC_MASTER_DATA, 0x04);
        outb(PIC_SLAVE_DATA,  0x02);
        outb(PIC_MASTER_DATA, 0x01);
        outb(PIC_SLAVE_DATA,  0x01);
        outb(PIC_MASTER_DATA,  0x0);
        outb(PIC_SLAVE_DATA,   0x0);

        // Unmask interrupts that are supported
        pic_mask(0, PIC_UNMASK);
#if CONFIG_DRV_KEYBOARD
        pic_mask(1, PIC_UNMASK);
#endif
}

void pic_mask(int irq, short mask)
{
        uint16_t port;
        uint8_t val;

        if (irq < 8)
                port = PIC_MASTER_DATA;
        else {
                port = PIC_SLAVE_DATA;
                port -= 8;
        }

        if (mask == PIC_MASK)
                val = inb(port) | (1 << irq);
        else if (mask == PIC_UNMASK)
                val = inb(port) & ~(1 << irq);
        else
                val = inb(port);

        outb(port, val);
}
