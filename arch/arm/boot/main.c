/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <lib/string.h>
#include <lib/stdio.h>
#include <cmd/shell.h>

#ifdef CONFIG_MM_FF
#include <mm/ff.h>
#endif

#ifdef CONFIG_TEST
#include <test.h>
#endif

extern size_t kernel_begin;
extern size_t kernel_end;

/*
 * The entry point into the kernel
 */
void kernel_main()
{
	printk("Kernel loaded at 0x%x - 0x%x %dB\n", &kernel_begin, &kernel_end, &kernel_end - &kernel_begin);

	init_mm();

#ifdef CONFIG_TEST
	run_tests();
#endif

	start_interactive_shell();
}
