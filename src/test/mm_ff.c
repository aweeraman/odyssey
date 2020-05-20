/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#if CONFIG_MM_FF

#include <test.h>
#include <sys/ffmm.h>
#include <lib/string.h>
#include <lib/stdio.h>

#ifdef ARCH_X86
#define MEM_START_ADDR1 0x700000
#define MEM_END_ADDR1   0x700800
#define MEM_START_ADDR2 0x710000
#define MEM_END_ADDR2   0x710800
#elif ARCH_ARM
#define MEM_START_ADDR1 0x90700000
#define MEM_END_ADDR1   0x90700800
#define MEM_START_ADDR2 0x91700000
#define MEM_END_ADDR2   0x91700800
#endif

int ff_mm_1()
{
        int ret = 0;

        ff_mm_superblock_t *superblock = create_superblock(MEM_START_ADDR1,
                                                           MEM_START_ADDR1,
                                                           MEM_END_ADDR1);

        uint32_t *addr1 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr1 == NULL, "couldn't get an available frame 1");

        uint32_t *addr2 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr2 == NULL, "couldn't get an available frame 2");

        free_frame(superblock, addr2);

        uint32_t *addr3 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr3 != addr2, "same frame was not returned after freeing");

        uint32_t *addr4 = (uint32_t *) get_available_frame(superblock, FRAME_BLOCK_SIZE+1);
        FAIL_IF(addr4 != NULL, "returned frame for request larger than FRAME_BLOCK_SIZE");

        uint32_t *addr5 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr5 == NULL, "couldn't get an available frame 3");

        uint32_t *addr6 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr6 == NULL, "couldn't get an available frame 4");

        uint32_t *addr7 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr7 != NULL, "should return null, out of memory");

        free_frame(superblock, addr7);
        free_frame(superblock, addr6);
        free_frame(superblock, addr5);
        free_frame(superblock, addr2);
        free_frame(superblock, addr1);

        uint32_t *addr8 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr8 != addr1, "should be the same as the first frame");

        free_frame(superblock, addr8);

        return ret;
}

int ff_mm_2()
{
        int ret = 0;

        ff_mm_superblock_t *superblock = create_superblock(MEM_START_ADDR1,
                                                           MEM_START_ADDR1,
                                                           MEM_END_ADDR1);
        create_superblock((uint32_t) superblock, MEM_START_ADDR2, MEM_END_ADDR2);

        uint32_t *addr1 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr1 == NULL, "couldn't get an available frame 1");

        uint32_t *addr2 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr2 == NULL, "couldn't get an available frame 2");

        uint32_t *addr3 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr3 == NULL, "couldn't get an available frame 3");

        uint32_t *addr4 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr4 == NULL, "couldn't get an available frame 4");

        uint32_t *addr5 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr5 == NULL, "couldn't get an available frame 5");

        uint32_t *addr6 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr6 == NULL, "couldn't get an available frame 6");

        uint32_t *addr7 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr7 == NULL, "couldn't get an available frame 7");

        uint32_t *addr8 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr8 == NULL, "couldn't get an available frame 8");

        uint32_t *addr9 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr9 != NULL, "returned frame 9 even though out of memory");

        return ret;
}

int ff_mm_3()
{
        ff_mm_stats_t stats;
        int ret = 0;

        ff_mm_superblock_t *superblock = create_superblock(MEM_START_ADDR1,
                                                           MEM_START_ADDR1,
                                                           MEM_END_ADDR1);
        create_superblock((uint32_t) superblock, MEM_START_ADDR2, MEM_END_ADDR2);

        uint32_t *addr1 = (uint32_t *) get_available_frame(superblock, 100);
        FAIL_IF(addr1 == NULL, "couldn't get an available frame 1");

        uint32_t *addr2 = (uint32_t *) get_available_frame(superblock, 150);
        FAIL_IF(addr2 == NULL, "couldn't get an available frame 2");

        stats = get_ff_mm_stats(superblock, &stats);
        FAIL_IF(stats.bytes_used != 250, "incorrect bytes_used");
        FAIL_IF(stats.total_bytes != 4096, "incorrect total_bytes");

        return ret;
}

#endif
