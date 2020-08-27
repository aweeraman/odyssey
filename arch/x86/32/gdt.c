/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/32/gdt.h>
#include <lib/stdio.h>
#include <lib/string.h>

static struct gdt_descriptor gdt;
static struct gdt_entry gdt_entries[GDT_ENTRIES];
static struct tss_entry tss;

static void gdt_entry(int32_t entry, int32_t base, int32_t limit,
	              uint8_t access, uint8_t flags)
{
	gdt_entries[entry].limit_lo = limit & 0xFFFF;
	gdt_entries[entry].base_lo  = base  & 0xFFFF;
	gdt_entries[entry].base_mid = (base >> 16)  & 0xFF;
	gdt_entries[entry].access   = access;
	gdt_entries[entry].limit_hi = (limit >> 16) & 0xF;
	gdt_entries[entry].flags    = flags & 0xF;
	gdt_entries[entry].base_hi  = (base >> 24)  & 0xFF;

	printk("  %d: base=%x limit=%x access=%b flags=%b\n",
	       entry,base, limit, access, flags);
}

static void tss_entry(size_t entry, uint8_t access, uint8_t flags,
		      uint32_t ss0, uint32_t esp0)
{
	memset(&tss, 0, sizeof(tss));

	uint32_t base = (uint32_t) &tss;
	uint32_t limit = sizeof(tss);

	gdt_entry(entry, base, limit, access, flags);

	tss.ss0  = ss0;
	tss.esp0 = esp0;
	tss.cs   = 0x0b;
	tss.ss   = tss.ds = tss.es = tss.fs = tss.gs = 0x13;
}

void set_kernel_stack(uint32_t stack)
{
	tss.esp0 = stack;
}

void gdt_init()
{
	printk("GDT [%d]\n", GDT_ENTRIES);

	gdt_entry(0, 0, 0, 0, 0);
	gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xC); /* kernel code segment */
	gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xC); /* kernel data segment */
	gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xC); /* user code segment */
	gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xC); /* user data segment */
	tss_entry(5, 0x89, 0xC, 0x10, 0x0);     /* tss */

	gdt.size   = sizeof(gdt_entries);
	gdt.offset = (uint32_t) &gdt_entries;

	load_gdt((uint32_t) (&gdt));
}
