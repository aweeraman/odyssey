/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef FFMM_H
#define FFMM_H

#include <stdint.h>
#include <stddef.h>

#define MEM_START_ADDR 0x300000 // initialize mm at the 3MB mark
#define MEM_FRAME_SIZE 5        // size of each frame of memory
#define MEM_END_ADDR   0x300100

#define FRAME_ROOT       1
#define FRAME_AVAILABLE  2

struct ff_mem_page {
        uint8_t flags;
        size_t  next;
        size_t  prev;
        uint8_t frame[MEM_FRAME_SIZE];
};

typedef struct ff_mem_page ff_mem_page_t;

void  init_ff_mm();
void* get_free_frame(size_t size);

#endif
