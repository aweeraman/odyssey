/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/boot/modules.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <sys/panic.h>

static int module_idx = 0;
static boot_module_t boot_modules[MAX_BOOT_MODULES];

void add_boot_module(uint32_t start, uint32_t end, char *name)
{
        if (module_idx >= MAX_BOOT_MODULES)
                panic("Maximum boot modules reached");

        boot_modules[module_idx].start = start;
        boot_modules[module_idx].end   = end;
        strncpy(boot_modules[module_idx].name, name, MODULE_NAME_MAX_LEN);

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
