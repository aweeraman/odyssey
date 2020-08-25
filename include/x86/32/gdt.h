/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_ENTRIES 5

struct gdt_descriptor {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed));

struct gdt_entry {
	uint16_t limit_lo;
	uint16_t base_lo;
	uint8_t  base_mid;
	uint8_t  access;
	uint8_t  limit_hi: 4;
	uint8_t  flags: 4;
	uint8_t  base_hi;
} __attribute__((packed));

void gdt_init();
void load_gdt();

#endif
