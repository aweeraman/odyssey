/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/panic.h>
#include <lib/stdio.h>

void panic(char *str)
{
	printk("PANIC: %s\r\n", str);

	while(1) {
#if ARCH_X86
		asm("hlt");
#endif
	}
}
