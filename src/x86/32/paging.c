/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/32/paging.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <x86/boot/modules.h>
#include <mm/ff.h>
#include <sys/tty.h>

extern uint32_t kernel_begin;
extern uint32_t kernel_end;

static uint32_t kernel_start_addr = (uint32_t) &kernel_begin;
static uint32_t kernel_end_addr   = (uint32_t) &kernel_end;

static page_dir_entry_t kernel_pg_dir __attribute__((aligned(PAGE_ALIGNMENT)));
static identity_map_entry_t identity_maps[MAX_IDENTITY_MAPS];
static int current_id_map_entry = 0;

void add_identity_map_region(uint32_t start, uint32_t end, char *desc,
			     char rw, char user)
{
	if (current_id_map_entry < MAX_IDENTITY_MAPS) {
		identity_maps[current_id_map_entry].start_addr = start;
		identity_maps[current_id_map_entry].end_addr   = end;
		strncpy(identity_maps[current_id_map_entry].description,
			desc, MAX_IDENTITY_MAP_DESCRIPTION_LEN-1);
		identity_maps[current_id_map_entry].rw	 = rw;
		identity_maps[current_id_map_entry].user       = user;
		current_id_map_entry++;
	} else {
		printk("WARNING: max identity maps reached!\n");
	}
}

uint32_t get_physical_addr(page_dir_entry_t *dir, uint32_t phys_addr)
{
	uint32_t table_idx = phys_addr >> 22;
	uint32_t page_idx = (phys_addr >> 12) & 0x3ff;

	uint32_t virtual_addr = dir->tables[table_idx][page_idx].addr << 12;
	virtual_addr |= (phys_addr & 0xfff);

	return virtual_addr;
}

void identity_map_page(page_dir_entry_t *dir, uint32_t table, uint32_t page,
		       uint32_t phys_addr, char present, char rw, char user)
{
	dir->tables[table][page].addr      = phys_addr / 0x1000;
	dir->tables[table][page].present   = present;
	dir->tables[table][page].rw        = rw;
	dir->tables[table][page].user      = user;

	uint32_t tbl_address = (uint32_t) &dir->tables[table][0];
	dir->directory[table].addr      = tbl_address >> 12;
	dir->directory[table].present   = present;
	dir->directory[table].rw	= rw;
	dir->directory[table].user      = user;
}

void identity_map_region(page_dir_entry_t *dir, identity_map_entry_t map[])
{
	for (int i = 0; i < current_id_map_entry; i++) {
		uint32_t phys_cur = map[i].start_addr;
		printk("  Identity mapping %s [0x%x - 0x%x] [%c]\n",
				map[i].description, map[i].start_addr,
				map[i].end_addr,
				map[i].user == PAGE_USER ? 'U': 'K');

		while (phys_cur < map[i].end_addr) {
			uint32_t table_idx = phys_cur >> 22;
			uint32_t page_idx  = (phys_cur >> 12) & 0x3ff;

			identity_map_page(dir, table_idx, page_idx, phys_cur, 1,
					  map[i].rw, map[i].user);

			phys_cur += 0x1000;
		}
	}
}

#ifdef CONFIG_TEST
static void validate_identity_map_regions(page_dir_entry_t *dir,
					  identity_map_entry_t map[])
{
	printk("  Checking page mappings\n");
	for (int i = 0; i < current_id_map_entry; i++) {
		uint32_t phys_cur = map[i].start_addr;
		while (phys_cur < map[i].end_addr) {
			uint32_t virt_addr = get_physical_addr(dir, phys_cur);
			if (virt_addr != phys_cur) {
				printk("WARNING: identity map mismatch, %x != %x\n",
						virt_addr, phys_cur);
			}
			phys_cur += 0x1000;
		}
	}
}
#endif /* CONFIG_TEST */

void flush_page(uint32_t addr)
{
	asm volatile ("invlpg (%0)" : : "r"(addr) : "memory");
}

void init_paging()
{
	printk("Initializing paging: kernel page directory at 0x%x\n", &kernel_pg_dir);
	memset(&kernel_pg_dir, 0, sizeof(page_dir_entry_t));

	// Identity map the kernel address space
	add_identity_map_region(kernel_start_addr, kernel_end_addr, "kernel", 1, 0);

	// Identity map the kernel allocator storage
	identity_map_kernel_heap();

	// Identity map RGB framebuffer
	identity_map_framebuffer();

	// Identity map GRUB boot modules
	identity_map_modules();

	// Map and validate the pages
	identity_map_region(&kernel_pg_dir, identity_maps);

#ifdef CONFIG_TEST
	validate_identity_map_regions(&kernel_pg_dir, identity_maps);
#endif

	switch_page_directory((uint32_t *) kernel_pg_dir.directory);
	enable_paging();
}
