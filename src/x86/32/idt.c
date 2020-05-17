/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/32/idt.h>
#include <x86/32/isr.h>
#include <x86/32/pic.h>
#include <lib/termio.h>

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

        irq0_address  = (uint32_t) irq0;
        irq1_address  = (uint32_t) irq1;
        irq2_address  = (uint32_t) irq2;
        irq3_address  = (uint32_t) irq3;
        irq4_address  = (uint32_t) irq4;
        irq5_address  = (uint32_t) irq5;
        irq6_address  = (uint32_t) irq6;
        irq7_address  = (uint32_t) irq7;
        irq8_address  = (uint32_t) irq8;
        irq9_address  = (uint32_t) irq9;
        irq10_address = (uint32_t) irq10;
        irq11_address = (uint32_t) irq11;
        irq12_address = (uint32_t) irq12;
        irq13_address = (uint32_t) irq13;
        irq14_address = (uint32_t) irq14;
        irq15_address = (uint32_t) irq15;

        idt_entry(32, irq0_address);
        idt_entry(33, irq1_address);
        idt_entry(34, irq2_address);
        idt_entry(35, irq3_address);
        idt_entry(36, irq4_address);
        idt_entry(37, irq5_address);
        idt_entry(38, irq6_address);
        idt_entry(39, irq7_address);
        idt_entry(40, irq8_address);
        idt_entry(41, irq9_address);
        idt_entry(42, irq10_address);
        idt_entry(43, irq11_address);
        idt_entry(44, irq12_address);
        idt_entry(45, irq13_address);
        idt_entry(46, irq14_address);
        idt_entry(47, irq15_address);

        uint32_t addr = (uint32_t) idt_entries;
        idt.size = (sizeof (idt_entries) - 1);
        idt.offset_lo = (uint32_t) addr & 0xffff;
        idt.offset_hi = addr >> 16;

        load_idt((uint32_t) (&idt));
}
