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

#ifndef IO_H
#define IO_H

#include <stdint.h>

#define VGA_IDX_PORT  0x3D4
#define VGA_DATA_PORT (VGA_IDX_PORT+1)

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(uint8_t x, uint8_t width, uint8_t y);
uint16_t get_cursor_position(void);

#endif
