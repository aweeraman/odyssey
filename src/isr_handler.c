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

#include "isr.h"
#include "io.h"
#include "libk.h"

void irq0_handler()
{
        outb(0x20, 0x20);
}

void irq1_handler()
{
        char keycode;
        keycode = inb(0x60);
        printf(".\n");
        outb(0x20, 0x20);
}

void irq2_handler()
{
        outb(0x20, 0x20);
}

void irq3_handler()
{
        outb(0x20, 0x20);
}

void irq4_handler()
{
        outb(0x20, 0x20);
}

void irq5_handler()
{
        outb(0x20, 0x20);
}

void irq6_handler()
{
        outb(0x20, 0x20);
}

void irq7_handler()
{
        outb(0x20, 0x20);
}

// IRQ greater than 7 EOI must be sent to both master and slave
void irq8_handler()
{
        outb(0xA0, 0x20);
        outb(0x20, 0x20);
}

void irq9_handler()
{
        outb(0xA0, 0x20);
        outb(0x20, 0x20);
}

void irq10_handler()
{
        outb(0xA0, 0x20);
        outb(0x20, 0x20);
}

void irq11_handler()
{
        outb(0xA0, 0x20);
        outb(0x20, 0x20);
}

void irq12_handler()
{
        outb(0xA0, 0x20);
        outb(0x20, 0x20);
}

void irq13_handler()
{
        outb(0xA0, 0x20);
        outb(0x20, 0x20);
}

void irq14_handler()
{
        outb(0xA0, 0x20);
        outb(0x20, 0x20);
}

void irq15_handler()
{
        outb(0xA0, 0x20);
        outb(0x20, 0x20);
}
