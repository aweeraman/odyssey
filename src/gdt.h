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

#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_ENTRIES 3

struct gdt_descriptor {
        uint16_t size;
        uint32_t offset;
} __attribute__((packed));

typedef struct gdt_descriptor gdt_descriptor_t;

struct gdt_entry {
        uint16_t limit_lo;
        uint16_t base_lo;
        uint8_t  base_mid;
        uint8_t  access;
        uint8_t  limit_hi: 4;
        uint8_t  flags: 4;
        uint8_t  base_hi;
} __attribute__((packed));

typedef struct gdt_entry gdt_entry_t;

void gdt_init();
extern void load_gdt();

#endif
