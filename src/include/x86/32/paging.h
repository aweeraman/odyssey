/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

typedef struct page_dir_entry {
        uint32_t present         : 1;
        uint32_t rw              : 1;
        uint32_t user            : 1;
        uint32_t write_through   : 1;
        uint32_t disable_cache   : 1;
        uint32_t accessed        : 1;
        uint32_t dirty           : 1;
        uint32_t page_size       : 1;
        uint32_t global          : 1;
        uint32_t reserved        : 3;
        uint32_t page_table_addr : 20;
}__attribute__((packed)) page_dir_entry_t;

#endif
