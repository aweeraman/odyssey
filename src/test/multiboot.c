/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef ARCH_X86

#include <test.h>
#include <sys/memory.h>

int multiboot2_magic_1()
{
        int ret = 0;
        mem_ptr_t *p = (mem_ptr_t *) 0x100000;

        ASSERT(p->word.w1 != 0x50d6, "peek at mem location 0x100000 is not 50d6");
        ASSERT(p->word.w2 != 0xe852, "peek at mem location 0x100002 is not e852");

        return ret;
}

#endif
