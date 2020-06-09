/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

typedef struct page_entry {
        uint32_t present         : 1;  /* must be 1 to map to a page */
        uint32_t rw              : 1;  /* if 0, writes not allowed */
        uint32_t user            : 1;  /* if 0, user mode access not allowed */
        uint32_t accessed        : 1;  /* indicates whether page has been accessed */
        uint32_t dirty           : 1;  /* indicates if page has been written to */
        uint32_t reserved        : 7;  /* ignored */
        uint32_t addr            : 20; /* physical address of 4KB page */
}__attribute__((packed)) page_entry_t;

typedef struct page_dir_entry {
        page_entry_t tables[1024];
        page_entry_t directory[1024];
}__attribute__((packed)) page_dir_entry_t;

void init_paging           ();
void enable_paging         ();
void switch_page_directory (uint32_t *addr);
void map_physical_memory   (page_dir_entry_t *dir, uint32_t phys_start, uint32_t phys_end);

#endif
