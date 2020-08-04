/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <mm/ff.h>
#include <lib/stdio.h>
#include <lib/string.h>

#if ARCH_X86
#include <x86/32/paging.h>
#elif ARCH_ARM
#define PAGE_ALIGNMENT 4096
#endif

static size_t heap_start_addr, heap_end_addr;

extern size_t kernel_end;
static size_t kernel_end_addr = (size_t) &kernel_end;

size_t test_heap_start_addr1;
size_t test_heap_start_addr2;

static struct mm_superblock *superblock;

#if ARCH_X86
void identity_map_kernel_heap()
{
	add_identity_map_region(heap_start_addr,
			        heap_end_addr,
				"kernel heap", PAGE_RW, PAGE_KERNEL);
}

#if CONFIG_TEST
void identity_map_kernel_test_heap()
{
	add_identity_map_region(test_heap_start_addr1,
			        test_heap_start_addr1 + PAGE_ALIGNMENT,
				"kernel test heap 1", PAGE_RW, PAGE_KERNEL);

	add_identity_map_region(test_heap_start_addr2,
			        test_heap_start_addr2 + PAGE_ALIGNMENT*4, /* TODO why *4? */
				"kernel test heap 2", PAGE_RW, PAGE_KERNEL);
}
#endif /* CONFIG_TEST */
#endif /* ARCH_X86 */

struct mm_stats get_mm_stats(struct mm_superblock *sb, struct mm_stats *stats)
{
	if (sb == NULL)
		sb = superblock;

	stats->bytes_used  = 0;
	stats->total_bytes = 0;

	do {
		for (size_t i = 0; i < sb->block_count; i++) {
			if (sb->blocks[i].flags == FRAME_INUSE ||
			    sb->blocks[i].flags == FRAME_MULTI_START ||
			    sb->blocks[i].flags == FRAME_MULTI) {
				stats->bytes_used += sb->blocks[i].bytes_used;
			}
			stats->total_bytes += FRAME_BLOCK_SIZE;
		}

		sb = sb->next_super_block;
	} while (sb != NULL);

	return (struct mm_stats) (*stats);
}

static void mark_available(struct mm_frame *frame)
{
	frame->flags      = FRAME_AVAILABLE;
	frame->bytes_used = 0;
	memset(frame->addr, '\0', FRAME_BLOCK_SIZE);
}

void free_frame(struct mm_superblock *sb, size_t *addr)
{
	if (sb == NULL)
		sb = superblock;

	do {
		int multi_start = 0;
		for (size_t i = 0; i < sb->block_count; i++) {
			size_t frame_addr = (size_t) sb->blocks[i].addr;
			int flags = sb->blocks[i].flags;

			if (frame_addr == (size_t) addr && flags == FRAME_MULTI_START) {
				multi_start = 1;
				mark_available(&sb->blocks[i]);
			} else if (frame_addr == (size_t) addr && flags == FRAME_INUSE) {
				mark_available(&sb->blocks[i]);
			} else if (multi_start == 1 && flags == FRAME_MULTI) {
				mark_available(&sb->blocks[i]);
			} else if (multi_start == 1 &&
				  (flags == FRAME_MULTI_START
				   || flags == FRAME_INUSE
				   || flags == FRAME_AVAILABLE)) {
				multi_start = 0;
			}
		}

		sb = sb->next_super_block;
	} while (sb != NULL);
}

void *get_available_frame(struct mm_superblock *sb, size_t size)
{
	void *ptr = NULL;

	if (sb == NULL)
		sb = superblock;

	do {
		if (size < FRAME_BLOCK_SIZE) {
			for (size_t i = 0; i < sb->block_count; i++) {
				if (sb->blocks[i].flags == FRAME_AVAILABLE) {
					sb->blocks[i].flags      = FRAME_INUSE;
					sb->blocks[i].bytes_used = size;
					memset(sb->blocks[i].addr, '\0', FRAME_BLOCK_SIZE);
					return (void *) sb->blocks[i].addr;
				}
			}
		} else {
			int start_block       = -1;
			int last_block	      = -1;
			int contiguous_blocks = 0;
			int size_remaining    = size;

			for (size_t i = 0; i < sb->block_count; i++) {
				if (sb->blocks[i].flags == FRAME_AVAILABLE) {
					if (start_block == -1) {
						start_block     = i;
						size_remaining -= FRAME_BLOCK_SIZE;
						last_block      = i;
						contiguous_blocks++;
					} else {
						if (last_block == (int) (i - 1)) {
							size_remaining -= FRAME_BLOCK_SIZE;
							last_block      = i;
							contiguous_blocks++;

							if (size_remaining <= 0)
								break;
						} else {
							start_block       = -1;
							last_block        = -1;
							contiguous_blocks = 0;
							size_remaining    = size;
						}
					}
				}
			}

			if (size_remaining > 0)
				return NULL;

			if (start_block != -1) {
				size_remaining = size;
				for (int i = start_block; i < contiguous_blocks; i++) {
					if (sb->blocks[i].flags == FRAME_AVAILABLE) {

						if (i == start_block)
							sb->blocks[i].flags = FRAME_MULTI_START;
						else
							sb->blocks[i].flags = FRAME_MULTI;

						if (size_remaining > FRAME_BLOCK_SIZE)
							sb->blocks[i].bytes_used = FRAME_BLOCK_SIZE;
						else
							sb->blocks[i].bytes_used = size_remaining;

						size_remaining -= FRAME_BLOCK_SIZE;
						memset(sb->blocks[i].addr, '\0', FRAME_BLOCK_SIZE);
					}
				}
				return (void *) sb->blocks[start_block].addr;
			}
		}

		sb = sb->next_super_block;
	} while (sb != NULL);

	return ptr;
}

struct mm_superblock *create_superblock(size_t root_block,
				        size_t start_addr,
				        size_t end_addr)
{
	size_t available_memory;
	size_t blocks;
	struct mm_superblock *sb;

	sb = (struct mm_superblock *) (size_t) root_block;

	if (root_block == start_addr) {
		memset(sb, '\0', sizeof(struct mm_superblock));
	} else {
		while (sb->next_super_block != NULL)
		       sb = sb->next_super_block;

		sb->next_super_block = (struct mm_superblock *) (size_t) start_addr;
		sb = sb->next_super_block;
		memset(sb, '\0', sizeof(struct mm_superblock));
	}

	sb->start_addr = (start_addr + sizeof(struct mm_superblock) + 1 + 7) & ~7;
	sb->next_super_block = NULL;

	available_memory = end_addr - start_addr;
	blocks = (size_t) available_memory / FRAME_BLOCK_SIZE;

	sb->block_count = blocks;

	for (size_t i = 0; i < sb->block_count; i++) {
		sb->blocks[i].addr = (size_t *) ((size_t) sb->start_addr)
						  + (FRAME_BLOCK_SIZE * i);
		mark_available(&sb->blocks[i]);
	}

	return sb;
}

void print_superblocks(struct mm_superblock *sb)
{
	int count = 0;

	if (sb == NULL)
		sb = superblock;

	do {
		printk("  %d: start: 0x%x, blocks: %d, block_size: %dB\n",
			count++,
			sb->start_addr,
			sb->block_count,
			FRAME_BLOCK_SIZE);
		sb = sb->next_super_block;
	} while (sb != NULL);
}

void init_mm()
{

	heap_start_addr       = (kernel_end_addr + 0x100000 + PAGE_ALIGNMENT)
					& ~(PAGE_ALIGNMENT - 1);
	heap_end_addr         = heap_start_addr + 0x100000;

#if CONFIG_TEST
	test_heap_start_addr1 = (heap_end_addr + PAGE_ALIGNMENT)
					& ~(PAGE_ALIGNMENT - 1);

	test_heap_start_addr2 = (test_heap_start_addr1 + PAGE_ALIGNMENT)
					& ~(PAGE_ALIGNMENT - 1);
#endif

	printk("Initializing FirstFit memory manager: 0x%x - 0x%x\n",
		heap_start_addr, heap_end_addr);
	superblock = create_superblock(heap_start_addr,
				       heap_start_addr,
				       heap_end_addr);
	print_superblocks(NULL);
}
