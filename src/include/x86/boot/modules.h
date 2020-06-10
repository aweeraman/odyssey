/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MODULES_H
#define MODULES_H

#include <stdint.h>

#define MAX_BOOT_MODULES    10
#define MODULE_NAME_MAX_LEN 128
#define CANARY_MAGIC_STRING 0x0dee5eee

typedef struct boot_module {
        uint32_t start;
        uint32_t end;
        char     name[MODULE_NAME_MAX_LEN];
} boot_module_t;

typedef void (*module_t)(void);

void     add_boot_module        (uint32_t start, uint32_t end, char *name);
uint32_t get_module_by_idx      (int idx);
void     print_boot_modules     ();
void     identity_map_modules   ();

#endif
