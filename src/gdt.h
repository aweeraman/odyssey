/**
 * SPDX-License-Identifier: GPL-3.0-or-later
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
