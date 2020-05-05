/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2020, Anuradha Weeraman
 *
 * This program includes code that is Copyright (C) 1999, 2010 Free
 * Software Foundation, Inc.
 */

#include "kernel.h"
#include "libk.h"
#include "boothdr.h"
#include "memory.h"
#include "gdt.h"
#include "pic.h"
#include "idt.h"
#include "shell.h"
#include "timer.h"

#ifdef CONFIG_TEST
#include "test.h"
#endif

#if defined CONFIG_MM && CONFIG_MM == flat
#include "flatmm.h"
#endif

#ifdef CONFIG_DRV_KEYBOARD
#include "keyboard.h"
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

#ifdef CONFIG_TEST
        run_tests();
#endif

        register_n_ticks_timer(5, blink_cursor);

        start_interactive_shell();
}
