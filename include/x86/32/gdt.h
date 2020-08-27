/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_ENTRIES 6

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

struct tss_entry
{
	uint32_t prev_tss;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} __attribute__((packed));

void gdt_init();
void load_gdt();
void set_kernel_stack();
void flush_tss();
void user_mode_init();
void user_function();

#endif
