/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/ffmm.h>
#include <lib/stdio.h>
#include <lib/string.h>

static ff_mem_page_t *root_frame = (ff_mem_page_t *) MEM_START_ADDR;

void* get_free_frame(size_t size)
{
        void* ptr = NULL;

        if (size > MEM_FRAME_SIZE) {
                return NULL;
        } else if (root_frame->flags &= FRAME_AVAILABLE) {
                root_frame->flags ^= FRAME_AVAILABLE;
                ptr = root_frame->frame;
        } else {
                return NULL;
        }

        return ptr;
}

void create_root_frame()
{
        root_frame->flags |= FRAME_ROOT | FRAME_AVAILABLE;
        root_frame->next = (size_t) root_frame+1;
        root_frame->prev = 0;
        memset(root_frame->frame, '\0', MEM_FRAME_SIZE);
}

void init_ff_mm()
{
        printf("Initializing FirstFit memory manager at 0x%x\n", MEM_START_ADDR);
        create_root_frame();
}
