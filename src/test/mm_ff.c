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
        char* frame;

        frame = get_free_frame(MEM_FRAME_SIZE+1);
        ASSERT(frame != NULL, "allocate frame larger than the max frame size");

        frame = get_free_frame(MEM_FRAME_SIZE-1);
        memset(frame, 'a', MEM_FRAME_SIZE-1);
        strncpy(frame, "123", MEM_FRAME_SIZE-1);
        ASSERT(!(frame[0] == '1' && frame[1] == '2' &&
                 frame[2] == '3' && frame[3] == '\0'), "unable to allocate string");

        frame = get_free_frame(MEM_FRAME_SIZE-1);
        ASSERT(frame != NULL, "frame should not be available");

        return ret;
}

#endif
