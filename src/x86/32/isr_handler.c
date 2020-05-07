/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/32/isr.h>
#include <x86/io.h>
#include <lib/k.h>
#include <x86/32/pic.h>
#include <sys/keyboard.h>
#include <sys/timer.h>

/*
 * Progammable Interrupt Timer
 */
void irq0_handler()
{
        tick();
        pic_eoi(0);
}

/*
 * Keyboard
 */
void irq1_handler()
{
#if CONFIG_DRV_KEYBOARD
        kbd_interrupt();
#endif
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
