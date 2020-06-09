/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/32/paging.h>
#include <lib/string.h>
#include <lib/stdio.h>

#define PAGE_ALIGNMENT     4096
#define PAGE_DIR_ENTRIES   1024
#define PAGE_TABLE_ENTRIES 1024

static page_dir_entry_t kernel_pg_dir __attribute__((aligned(PAGE_ALIGNMENT)));

static void map_page(page_dir_entry_t *dir, uint32_t table, uint32_t page,
                     uint32_t phys_addr, char present, char rw, char user)
{

        dir->tables[page].addr      = phys_addr >> 12;
        dir->tables[page].present   = present;
        dir->tables[page].rw        = rw;
        dir->tables[page].user      = user;

        dir->directory[table].addr      = (uint32_t) dir->tables >> 12;
        dir->directory[table].present   = present;
        dir->directory[table].rw        = rw;
        dir->directory[table].user      = user;
}

uint32_t get_virtual_addr(page_dir_entry_t *dir, uint32_t phys_addr)
{
        uint32_t table_idx = phys_addr >> 22;
        uint32_t page_idx = (phys_addr >> 12) & 0x3ff;
        uint32_t virtual_addr = dir->tables[page_idx].addr << 12;
        virtual_addr |= (phys_addr & 0xfff);
        return virtual_addr;
}

void map_physical_memory(page_dir_entry_t *dir, uint32_t phys_start, uint32_t phys_end)
{
        uint32_t phys_cur = phys_start;

        printk("  Identity mapping 0x%x - 0x%x\n", phys_start, phys_end);
        while (phys_cur < phys_end) {
                uint32_t table_idx = phys_cur >> 22;
                uint32_t page_idx = (phys_cur >> 12) & 0x3ff;

                map_page(dir, table_idx, page_idx, phys_cur, 1, 1, 0);

                phys_cur += 0x1000;
        }
}

void init_paging()
{
        printk("Initializing paging: kernel page directory at 0x%x\n", &kernel_pg_dir);

        memset(&kernel_pg_dir, 0, sizeof(page_dir_entry_t));

        map_physical_memory(&kernel_pg_dir, 0x0, 0x410000);

        //map_physical_memory(&kernel_pg_dir, 0xfc000000, 0xfe000000);
        //printk("DEBUG %x\n", get_virtual_addr(&kernel_pg_dir, 0xfd000000));

        switch_page_directory((uint32_t *) kernel_pg_dir.directory);

        enable_paging();
}
