/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <lib/stdio.h>
#include <x86/boot/boothdr.h>
#include <x86/boot/device.h>
#include <sys/memory.h>
#include <x86/32/gdt.h>
#include <x86/32/pic.h>
#include <x86/32/idt.h>
#include <x86/32/acpi.h>
#include <cmd/shell.h>
#include <sys/timer.h>

#ifdef CONFIG_SERIAL
#include <sys/serial.h>
#endif

#ifdef CONFIG_TEST
#include <test.h>
#endif

#ifdef CONFIG_MM_FF
#include <sys/ffmm.h>
#endif

#ifdef CONFIG_KEYBOARD
#include <sys/keyboard.h>
#endif

extern uintptr_t kernel_begin;
extern uintptr_t kernel_end;

extern void blink_cursor();

struct boot_device        *boot_dev;
struct acpi_descriptor_v1 *acpi_v1;
struct acpi_descriptor_v2 *acpi_v2;

/*
 * The entry point into the kernel
 */
void kernel_main()
{

#ifdef CONFIG_SERIAL
        init_serial();
#endif

        read_multiboot_header_tags();

#ifdef CONFIG_SERIAL
        printk("Initialized serial interface\n");
#endif

        printk("Kernel loaded at 0x%x - 0x%x %dB\n", &kernel_begin, &kernel_end, &kernel_end - &kernel_begin);

        pic_unmask_interrupts();

#ifdef CONFIG_KEYBOARD
        keyboard_init();
#endif

#ifdef CONFIG_MM_FF
        init_ff_mm();
#endif

        printk("Loops per tick: %d\n", get_loops_per_tick());

#ifdef CONFIG_TEST
        run_tests();
#endif

#ifdef CONFIG_FRAMEBUFFER_RGB
        register_n_ticks_timer(5, blink_cursor);
#endif

        start_interactive_shell();
}
