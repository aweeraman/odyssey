/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <io.h>
#include <timer.h>

extern uint32_t ticks;

void outb(uint16_t port, uint8_t val)
{
        asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(uint16_t port)
{
        uint8_t ret;
        asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
}

void io_wait()
{
        uint32_t end = ticks + 2;
        while (ticks < end);
}
