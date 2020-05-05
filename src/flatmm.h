/**
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright 2020, Anuradha Weeraman
 */

#ifndef FLATMM_H
#define FLATMM_H

#include <stdint.h>
#include <stddef.h>

#define MEM_START_ADDR 0x300000 // initialize mm at the 3MB mark
#define MEM_FRAME_SIZE 5        // size of each frame of memory
#define MEM_END_ADDR   0x300100

#define FRAME_ROOT       1
#define FRAME_AVAILABLE  2

struct flat_mem_page {
        uint8_t flags;
        size_t  next;
        size_t  prev;
        uint8_t frame[MEM_FRAME_SIZE];
};

typedef struct flat_mem_page flat_mem_page_t;

void  init_flatmm();
void* get_free_frame(size_t size);

#endif
