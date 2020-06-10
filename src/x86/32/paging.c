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

        dir->tables[table][page].addr      = phys_addr / 0x1000;
        dir->tables[table][page].present   = present;
        dir->tables[table][page].rw        = rw;
        dir->tables[table][page].user      = user;

        uint32_t tbl_address = (uint32_t) &dir->tables[table][0];
        dir->directory[table].addr      = tbl_address >> 12;
        dir->directory[table].present   = present;
        dir->directory[table].rw        = rw;
        dir->directory[table].user      = user;
}

uint32_t get_virtual_addr(page_dir_entry_t *dir, uint32_t phys_addr)
{
        uint32_t table_idx = phys_addr >> 22;
        uint32_t page_idx = (phys_addr >> 12) & 0x3ff;
        uint32_t virtual_addr = dir->tables[table_idx][page_idx].addr << 12;
        virtual_addr |= (phys_addr & 0xfff);

        printk("Physical addr=%x, table=%x, page=%x, idx=%x, virtual addr=%x\n",
               phys_addr, table_idx, page_idx, (phys_addr & 0xfff), virtual_addr);

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

        // Identity map the kernel. Right now the page tables are statically
        // allocated, optimize this with dynamically allocating them based on
        // need to reduce the memory footprint
        map_physical_memory(&kernel_pg_dir, 0x0       , 0x1500000);

        // Identity map RGB framebuffer, good enough for 1024x768x32
        map_physical_memory(&kernel_pg_dir, 0xfd000000, 0xfd300000);

        switch_page_directory((uint32_t *) kernel_pg_dir.directory);

        enable_paging();
}
