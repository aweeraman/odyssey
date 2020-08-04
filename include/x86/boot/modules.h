/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MODULES_H
#define MODULES_H

#include <stddef.h>

#define MAX_BOOT_MODULES    10
#define MODULE_NAME_MAX_LEN 128
#define CANARY_MAGIC_STRING 0xdc0dc0dc

struct boot_module {
	size_t start;
	size_t end;
	char   name[MODULE_NAME_MAX_LEN];
};

void add_boot_module(size_t start, size_t end, char *name);
void *get_module_by_idx(int idx);
void print_boot_modules();
void identity_map_modules();

#endif
