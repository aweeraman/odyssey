/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <lib/string.h>
#include <lib/stdio.h>

#ifdef CONFIG_MM_FF
#include <mm/ff.h>
#endif

#ifdef CONFIG_TEST
#include <test.h>
#endif

void kernel_main() {
	printk("\nOdyssey v%s\n",
			STRINGIFY(CONFIG_VERSION_MAJOR) "." \
			STRINGIFY(CONFIG_VERSION_MINOR));

	init_mm();

#ifdef CONFIG_TEST
	run_tests();
#endif
}
