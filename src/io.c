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
 *
 * Inline Assembly: https://wiki.osdev.org/Inline_Assembly
 * Text Mode Cursor Reference: https://wiki.osdev.org/Text_Mode_Cursor
 * VGA Hardware: https://wiki.osdev.org/VGA_Hardware
 */

#include "io.h"
#include "timer.h"

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
