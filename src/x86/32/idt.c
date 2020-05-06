/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/32/idt.h>
#include <x86/32/isr.h>
#include <x86/32/pic.h>
#include <lib/k.h>

static idt_descriptor_t idt;
static idt_entry_t idt_entries[256];

static void idt_entry(int idx, uint32_t offset)
{
        idt_entries[idx].offset_lo = offset & 0xffff;
        idt_entries[idx].selector = 0x08;
        idt_entries[idx].zero = 0;
        idt_entries[idx].type_attr = 0x8e;
        idt_entries[idx].offset_hi = (offset & 0xffff0000) >> 16;
}

void idt_init()
{
        printf("Initializing interrupt vector table\n");
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
        idt.size = (sizeof (idt_entries) - 1);
        idt.offset_lo = (uint32_t) addr & 0xffff;
        idt.offset_hi = addr >> 16;

        load_idt((uint32_t) (&idt));
}
