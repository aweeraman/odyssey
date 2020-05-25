/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

typedef struct page_dir_entry {
        uint32_t present         : 1;  /* must be 1 to reference a page table */
        uint32_t rw              : 1;  /* if 0, writes not allowed to the 4MB region controlled by this entry */
        uint32_t user            : 1;  /* if 0, user mode accesses not allowed to the 4MB region */
        uint32_t write_through   : 1;  /* page level write through cache */
        uint32_t disable_cache   : 1;  /* page level cache disable */
        uint32_t accessed        : 1;  /* indicates whether this entry has been used for access */
        uint32_t ignored         : 1;  /* ignored */
        uint32_t page_size       : 1;  /* if CR4.PSE = 1, must be zero, otherwise ignored */
        uint32_t reserved        : 4;  /* ignored */
        uint32_t page_table_addr : 20; /* physical address of 4KB aligned page table */
}__attribute__((packed)) page_dir_entry_t;

typedef struct page_table_entry {
        uint32_t present         : 1;  /* must be 1 to map to a page */
        uint32_t rw              : 1;  /* if 0, writes not allowed */
        uint32_t user            : 1;  /* if 0, user mode access not allowed */
        uint32_t write_through   : 1;  /* page level write through */
        uint32_t disable_cache   : 1;  /* page level cache disable */
        uint32_t accessed        : 1;  /* indicates whether page has been accessed */
        uint32_t dirty           : 1;  /* indicates if page has been written to */
        uint32_t memory_type     : 1;  /* zero unless PAT is supported */
        uint32_t global          : 1;  /* ignored unless CR4.PGE = 1 */
        uint32_t reserved        : 3;  /* ignored */
        uint32_t page_addr       : 20; /* physical address of 4KB page */
}__attribute__((packed)) page_table_entry_t;

#endif
