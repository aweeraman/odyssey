/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MMFF_H
#define MMFF_H

#include <stdint.h>
#include <stddef.h>

#define FRAME_AVAILABLE   1
#define FRAME_INUSE       2
#define FRAME_MULTI_START 4
#define FRAME_MULTI       8
#define FRAME_BLOCK_COUNT 1024
#define FRAME_BLOCK_SIZE  512

struct mm_stats {
	size_t bytes_used;
	size_t total_bytes;
};

struct mm_frame {
	uint8_t  flags;
	size_t  *addr;
	size_t   bytes_used;
}__attribute__((packed));

struct mm_superblock {
	size_t start_addr;
	size_t block_count;
	struct mm_superblock *next_super_block;
	struct mm_frame blocks[FRAME_BLOCK_COUNT];
}__attribute__((packed));

void init_mm();
void *get_available_frame(struct mm_superblock *sb, size_t size);
void free_frame(struct mm_superblock *sb, size_t *addr);
void print_superblocks(struct mm_superblock *sb);
struct mm_stats get_mm_stats(struct mm_superblock *sb, struct mm_stats *stats);
struct mm_superblock *create_superblock(size_t root_block,
				        size_t start_addr,
				        size_t end_addr);
void identity_map_kernel_heap();
void identity_map_kernel_test_heap();

#endif
