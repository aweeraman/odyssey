/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/32/gdt.h>
#include <lib/stdio.h>

static struct gdt_descriptor gdt;
static struct gdt_entry gdt_entries[GDT_ENTRIES];

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

void gdt_init()
{
	printk("GDT [%d]\n", GDT_ENTRIES);

	gdt_entry(0, 0, 0, 0, 0);
	gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xC); /* kernel code segment */
	gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xC); /* kernel data segment */
	gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xC); /* user code segment */
	gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xC); /* user data segment */

	gdt.size   = sizeof(gdt_entries);
	gdt.offset = (uint32_t) &gdt_entries;

	load_gdt((uint32_t) (&gdt));
}
