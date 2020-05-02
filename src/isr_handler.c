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
#include "pic.h"

/*
 * Progammable Interrupt Timer
 */
void irq0_handler()
{
        pic_eoi(0);
}

/*
 * Keyboard
 */
void irq1_handler()
{
        char keycode;
        keycode = inb(0x60);
        printf("%d\n", keycode);
        pic_eoi(1);
}

/*
 * Used internally by the two PICs. Never raised.
 */
void irq2_handler()
{
        pic_eoi(2);
}

/*
 * COM2
 */
void irq3_handler()
{
        pic_eoi(3);
}

/*
 * COM1
 */
void irq4_handler()
{
        pic_eoi(4);
}

/*
 * LPT2
 */
void irq5_handler()
{
        pic_eoi(5);
}

/*
 * Floppy disk
 */
void irq6_handler()
{
        pic_eoi(6);
}

/*
 * LPT1 / unreliable "spurious" interrupt
 */
void irq7_handler()
{
        pic_eoi(7);
}

/*
 * CMOS real-time clock
 */
void irq8_handler()
{
        pic_eoi(8);
}

/*
 * Free for peripherals / legacy SCSCI / NIC
 */
void irq9_handler()
{
        pic_eoi(9);
}

/*
 * Free for peripherals / SCSI / NIC
 */
void irq10_handler()
{
        pic_eoi(10);
}

/*
 * Free for peripherals / SCSI / NIC
 */
void irq11_handler()
{
        pic_eoi(11);
}

/*
 * PS/2: auxiliary device
 */
void irq12_handler()
{
        printf(".");
        pic_eoi(12);
}

/*
 * FPU / Coprocessor / Inter-processor
 */
void irq13_handler()
{
        pic_eoi(13);
}

/*
 * Primary ATA hard disk
 */
void irq14_handler()
{
        pic_eoi(14);
}

/*
 * Secondary ATA hard disk
 */
void irq15_handler()
{
        pic_eoi(15);
}
