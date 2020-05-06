/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <kernel.h>
#include <libk.h>
#include <boot/boothdr.h>
#include <memory.h>
#include <x86/32/gdt.h>
#include <x86/32/pic.h>
#include <x86/32/idt.h>
#include <cmd/shell.h>
#include <timer.h>

#ifdef CONFIG_TEST
#include <test.h>
#endif

#if defined CONFIG_MM && CONFIG_MM == flat
#include <mm/flatmm.h>
#endif

#ifdef CONFIG_DRV_KEYBOARD
#include <keyboard.h>
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
void kernel_main(size_t magic, size_t addr)
{

#ifdef CONFIG_SERIAL
        init_serial();
#endif

        early_framebuffer_console_init(magic, addr);

#ifdef CONFIG_SERIAL
        printf("Initialized serial at %s\n", STRINGIFY(CONFIG_SERIAL));
#endif

        printf("Kernel loaded at 0x%x - 0x%x %dB\n", &kernel_begin, &kernel_end, &kernel_end - &kernel_begin);

        gdt_init();
        pic_init();
        idt_init();

#ifdef CONFIG_DRV_KEYBOARD
        keyboard_init();
#endif

        read_multiboot_header_tags(magic, addr);

#if defined CONFIG_MM && CONFIG_MM == flat
        init_flatmm();
#endif

        printf("Loops per tick: %d\n", get_loops_per_tick());

#ifdef CONFIG_TEST
        run_tests();
#endif

#if CONFIG_FRAMEBUFFER_RGB
        register_n_ticks_timer(5, blink_cursor);
#endif

        start_interactive_shell();
}
