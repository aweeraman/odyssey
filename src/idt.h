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

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_descriptor {
        uint16_t size;
        uint16_t offset_lo;
        uint16_t offset_hi;
} __attribute__((packed));

typedef struct idt_descriptor idt_descriptor_t;

struct idt_entry {
        uint16_t offset_lo;
        uint16_t selector;
        uint8_t  zero;
        uint8_t  type_attr;
        uint16_t offset_hi;
} __attribute__((packed));

typedef struct idt_entry idt_entry_t;

void idt_init();
extern void load_idt();

#endif
