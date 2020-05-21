/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef FFMM_H
#define FFMM_H

#include <stdint.h>
#include <stddef.h>

#define FRAME_AVAILABLE   0
#define FRAME_INUSE       1
#define FRAME_BLOCK_COUNT 1024
#define FRAME_BLOCK_SIZE  512

struct mm_stats {
        uint32_t bytes_used;
        uint32_t total_bytes;
};

struct mm_frame {
        uint8_t  flags;
        uint32_t addr;
        uint32_t bytes_used;
}__attribute__((packed));

struct mm_superblock {
        uint32_t start_addr;
        uint32_t block_count;
        struct   mm_superblock *next_super_block;
        struct   mm_frame blocks[FRAME_BLOCK_COUNT];
}__attribute__((packed));

typedef struct mm_superblock mm_superblock_t;
typedef struct mm_stats mm_stats_t;

void  init_mm();
void* get_available_frame(mm_superblock_t *sb, size_t size);
void  free_frame(mm_superblock_t *sb, uint32_t *addr);
void  print_superblocks(mm_superblock_t *sb);
mm_stats_t       get_mm_stats(mm_superblock_t *sb, mm_stats_t *stats);
mm_superblock_t *create_superblock(uint32_t root_block,
                                      uint32_t start_addr,
                                      uint32_t end_addr);

#endif
