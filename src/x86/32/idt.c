/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/32/idt.h>
#include <x86/32/isr.h>
#include <x86/32/pic.h>
#include <lib/stdio.h>

static idt_descriptor_t idt;
static idt_entry_t idt_entries[256];

static void idt_entry(int idx, uint32_t offset)
{
	idt_entries[idx].offset_lo = offset & 0xffff;
	idt_entries[idx].selector  = 0x08;
	idt_entries[idx].zero      = 0;
	idt_entries[idx].type_attr = 0x8e;
	idt_entries[idx].offset_hi = (offset & 0xffff0000) >> 16;
}

void idt_init()
{
	printk("Initializing interrupt vector table\n");

	idt_entry(0,  (uint32_t) exception0);
	idt_entry(1,  (uint32_t) exception1);
	idt_entry(2,  (uint32_t) exception2);
	idt_entry(3,  (uint32_t) exception3);
	idt_entry(4,  (uint32_t) exception4);
	idt_entry(5,  (uint32_t) exception5);
	idt_entry(6,  (uint32_t) exception6);
	idt_entry(7,  (uint32_t) exception7);
	idt_entry(8,  (uint32_t) exception8);
	idt_entry(9,  (uint32_t) exception9);
	idt_entry(10, (uint32_t) exception10);
	idt_entry(11, (uint32_t) exception11);
	idt_entry(12, (uint32_t) exception12);
	idt_entry(13, (uint32_t) exception13);
	idt_entry(14, (uint32_t) exception14);
	idt_entry(15, (uint32_t) exception15);
	idt_entry(16, (uint32_t) exception16);
	idt_entry(17, (uint32_t) exception17);
	idt_entry(18, (uint32_t) exception18);
	idt_entry(19, (uint32_t) exception19);
	idt_entry(20, (uint32_t) exception20);
	idt_entry(21, (uint32_t) exception21);
	idt_entry(22, (uint32_t) exception22);
	idt_entry(23, (uint32_t) exception23);
	idt_entry(24, (uint32_t) exception24);
	idt_entry(25, (uint32_t) exception25);
	idt_entry(26, (uint32_t) exception26);
	idt_entry(27, (uint32_t) exception27);
	idt_entry(28, (uint32_t) exception28);
	idt_entry(29, (uint32_t) exception29);
	idt_entry(30, (uint32_t) exception30);
	idt_entry(31, (uint32_t) exception31);

	idt_entry(32, (uint32_t) irq0);
	idt_entry(33, (uint32_t) irq1);
	idt_entry(34, (uint32_t) irq2);
	idt_entry(35, (uint32_t) irq3);
	idt_entry(36, (uint32_t) irq4);
	idt_entry(37, (uint32_t) irq5);
	idt_entry(38, (uint32_t) irq6);
	idt_entry(39, (uint32_t) irq7);
	idt_entry(40, (uint32_t) irq8);
	idt_entry(41, (uint32_t) irq9);
	idt_entry(42, (uint32_t) irq10);
	idt_entry(43, (uint32_t) irq11);
	idt_entry(44, (uint32_t) irq12);
	idt_entry(45, (uint32_t) irq13);
	idt_entry(46, (uint32_t) irq14);
	idt_entry(47, (uint32_t) irq15);

	uint32_t addr = (uint32_t) idt_entries;
	idt.size      = (sizeof (idt_entries) - 1);
	idt.offset_lo = (uint32_t) addr & 0xffff;
	idt.offset_hi = addr >> 16;

	load_idt((uint32_t) (&idt));
}
