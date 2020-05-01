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

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
  outb(VGA_IDX_PORT, 0x0A);
  outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xC0) | cursor_start);

  outb(VGA_IDX_PORT, 0x0B);
  outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xE0) | cursor_end);
}

void disable_cursor()
{
  outb(VGA_IDX_PORT, 0x0A);
  outb(VGA_DATA_PORT, 0x20);
}

void update_cursor(uint8_t x, uint8_t width, uint8_t y)
{
  uint16_t pos = (x * width) + y;

  outb(VGA_IDX_PORT, 0x0F);
  outb(VGA_DATA_PORT, (uint8_t) (pos & 0xFF));
  outb(VGA_IDX_PORT, 0x0E);
  outb(VGA_DATA_PORT, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void)
{
  uint16_t pos = 0;
  outb(VGA_IDX_PORT, 0x0F);
  pos |= inb(VGA_DATA_PORT);
  outb(VGA_IDX_PORT, 0x0E);
  pos |= ((uint16_t) inb(VGA_DATA_PORT)) << 8;
  return pos;
}
