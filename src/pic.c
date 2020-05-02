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
        pic_mask(1, PIC_UNMASK);
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
