/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
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
void load_idt();

#endif
