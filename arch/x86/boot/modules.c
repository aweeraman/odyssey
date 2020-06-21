/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/boot/modules.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <sys/panic.h>
#include <x86/32/paging.h>

static int module_idx = 0;
static struct boot_module boot_modules[MAX_BOOT_MODULES];

void identity_map_modules()
{
	for (int i = 0; i < module_idx; i++) {
		add_identity_map_region(boot_modules[i].start,
					boot_modules[i].end,
					boot_modules[i].name,
					PAGE_RW, PAGE_USER);
	}
}

void add_boot_module(uint32_t start, uint32_t end, char *name)
{
	if (module_idx >= MAX_BOOT_MODULES)
		panic("Maximum boot modules reached");

	boot_modules[module_idx].start = start;
	boot_modules[module_idx].end   = end;
	strncpy(boot_modules[module_idx].name, name, MODULE_NAME_MAX_LEN-1);

	module_idx++;
}

void print_boot_modules()
{
	if (module_idx > 0)
		printk("Boot modules:\n");

	for (int i = 0; i < module_idx; i++) {
		printk("  %d: %s [0x%x - 0x%x]\n",
			i, boot_modules[i].name, boot_modules[i].start,
			boot_modules[i].end);
	}

}

void *get_module_by_idx(int idx)
{
	if (idx < module_idx)
		return (void *) boot_modules[idx].start;

	return 0;
}
