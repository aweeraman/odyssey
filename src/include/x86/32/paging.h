/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_ALIGNMENT                   4096
#define PAGE_DIR_ENTRIES                 1024
#define PAGE_TABLE_ENTRIES               1024
#define MAX_IDENTITY_MAPS                10
#define MAX_IDENTITY_MAP_DESCRIPTION_LEN 50

#define PAGE_USER   1
#define PAGE_KERNEL 0
#define PAGE_RW     1
#define PAGE_RO     0

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
        page_entry_t tables[PAGE_DIR_ENTRIES][PAGE_TABLE_ENTRIES];
        page_entry_t directory[PAGE_DIR_ENTRIES];
}__attribute__((packed)) page_dir_entry_t;

typedef struct identity_map_entry {
        uint32_t start_addr;
        uint32_t end_addr;
        char     description[MAX_IDENTITY_MAP_DESCRIPTION_LEN];
        char     rw;
        char     user;
} identity_map_entry_t;

void     init_paging             ();
void     enable_paging           ();
void     switch_page_directory   (uint32_t *addr);
void     identity_map_page       (page_dir_entry_t *dir,
                                  uint32_t table, uint32_t page,
                                  uint32_t phys_addr, char present,
                                  char rw, char user);
void     identity_map_region     (page_dir_entry_t *dir,
                                  identity_map_entry_t map[]);
uint32_t get_virtual_addr        (page_dir_entry_t *dir, uint32_t phys_addr);
void     add_identity_map_region (uint32_t start, uint32_t end, char *desc,
                                  char rw, char user);
void     flush_page              (uint32_t addr);

#endif
