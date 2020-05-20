/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#if ARCH_X86 && CONFIG_MM_FF

#include <test.h>
#include <sys/ffmm.h>
#include <lib/string.h>

int ff_mm_1()
{
        int ret = 0;

        uint32_t *addr1 = (uint32_t *) get_available_frame(100);
        ASSERT(addr1 == NULL, "couldn't get an available frame 1");

        uint32_t *addr2 = (uint32_t *) get_available_frame(100);
        ASSERT(addr2 == NULL, "couldn't get an available frame 2");

        free_frame(addr2);

        uint32_t *addr3 = (uint32_t *) get_available_frame(100);
        ASSERT(addr3 != addr2, "same frame was not returned after freeing");

        uint32_t *addr4 = (uint32_t *) get_available_frame(FRAME_BLOCK_SIZE+1);
        ASSERT(addr4 != NULL, "returned frame for request larger than FRAME_BLOCK_SIZE");

        return ret;
}

#endif
