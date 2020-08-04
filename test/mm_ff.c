/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#if CONFIG_MM_FF

#include <test.h>
#include <mm/ff.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <lib/mm.h>

extern size_t test_heap_start_addr1;
extern size_t test_heap_start_addr2;

static size_t test_heap_end_addr1, test_heap_end_addr2;

int ff_mm_1()
{
	int ret = 0;

	test_heap_end_addr1 = test_heap_start_addr1 + 0x800;
	test_heap_end_addr2 = test_heap_start_addr2 + 0x800;

	struct mm_superblock *superblock = create_superblock(test_heap_start_addr1,
							     test_heap_start_addr1,
							     test_heap_end_addr1);

	size_t *addr1 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr1 == NULL, "couldn't get an available frame 1");

	size_t *addr2 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr2 == NULL, "couldn't get an available frame 2");

	free_frame(superblock, addr2);

	size_t *addr3 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr3 != addr2, "same frame was not returned after freeing");

	size_t *addr5 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr5 == NULL, "couldn't get an available frame 3");

	size_t *addr6 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr6 == NULL, "couldn't get an available frame 4");

	size_t *addr7 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr7 != NULL, "should return null, out of memory");

	free_frame(superblock, addr7);
	free_frame(superblock, addr6);
	free_frame(superblock, addr5);
	free_frame(superblock, addr2);
	free_frame(superblock, addr1);

	size_t *addr8 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr8 != addr1, "should be the same as the first frame");

	free_frame(superblock, addr8);

	return ret;
}

int ff_mm_2()
{
	int ret = 0;

	struct mm_superblock *superblock = create_superblock(test_heap_start_addr1,
						             test_heap_start_addr1,
						             test_heap_end_addr1);
	create_superblock((size_t) superblock, test_heap_start_addr2, test_heap_end_addr2);

	size_t *addr1 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr1 == NULL, "couldn't get an available frame 1");

	size_t *addr2 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr2 == NULL, "couldn't get an available frame 2");

	size_t *addr3 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr3 == NULL, "couldn't get an available frame 3");

	size_t *addr4 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr4 == NULL, "couldn't get an available frame 4");

	size_t *addr5 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr5 == NULL, "couldn't get an available frame 5");

	size_t *addr6 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr6 == NULL, "couldn't get an available frame 6");

	size_t *addr7 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr7 == NULL, "couldn't get an available frame 7");

	size_t *addr8 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr8 == NULL, "couldn't get an available frame 8");

	size_t *addr9 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr9 != NULL, "returned frame 9 even though out of memory");

	return ret;
}

int ff_mm_3()
{
	struct mm_stats stats;
	int ret = 0;

	struct mm_superblock *superblock = create_superblock(test_heap_start_addr1,
						             test_heap_start_addr1,
						             test_heap_end_addr1);
	create_superblock((size_t) superblock, test_heap_start_addr2, test_heap_end_addr2);

	size_t *addr1 = (size_t *) get_available_frame(superblock, 100);
	FAIL_IF(addr1 == NULL, "couldn't get an available frame 1");

	size_t *addr2 = (size_t *) get_available_frame(superblock, 150);
	FAIL_IF(addr2 == NULL, "couldn't get an available frame 2");

	stats = get_mm_stats(superblock, &stats);
	FAIL_IF(stats.bytes_used != 250, "incorrect bytes_used");
	FAIL_IF(stats.total_bytes != 4096, "incorrect total_bytes");

	return ret;
}

int ff_mm_4()
{
	int ret = 0;
	struct mm_stats stats;
	struct mm_superblock *superblock = create_superblock(test_heap_start_addr1,
						             test_heap_start_addr1,
						             test_heap_end_addr1);
	create_superblock((size_t) superblock, test_heap_start_addr2, test_heap_end_addr2);

	size_t *addr1 = (size_t *) kzalloc(superblock, sizeof(size_t), 100);
	FAIL_IF(addr1 == NULL, "couldn't get an available frame 1");

	size_t *addr2 = (size_t *) kzalloc(superblock, sizeof(size_t), 100);
	FAIL_IF(addr2 == NULL, "couldn't get an available frame 2");

	stats = get_mm_stats(superblock, &stats);
	FAIL_IF(stats.bytes_used != 800, "incorrect bytes_used 1");

	kzfree(superblock, addr1);

	size_t *addr3 = (size_t *) kzalloc(superblock, sizeof(size_t), 50);
	FAIL_IF(addr3 == NULL, "couldn't get an available frame 3");
	FAIL_IF(addr3 != addr1, "addr2 should equal addr1");

	stats = get_mm_stats(superblock, &stats);
	FAIL_IF(stats.bytes_used != 600, "incorrect bytes_used 2");
	FAIL_IF(stats.total_bytes != 4096, "incorrect total_bytes");

	return ret;
}

int ff_mm_5()
{
	int ret = 0;
	struct mm_superblock *superblock = create_superblock(test_heap_start_addr1,
							     test_heap_start_addr1,
							     test_heap_end_addr1);
	create_superblock((size_t) superblock, test_heap_start_addr2, test_heap_end_addr2);

	char *addr1 = (char *) kzalloc(superblock, sizeof(size_t), 100);
	FAIL_IF(addr1[0] != '\0', "kzalloc didn't zero the memory");

	strncpy(addr1, "123", 5);
	FAIL_IF(strncmp(addr1, "123", 5) != 0, "string didn't get set correctly");

	kzfree(superblock, addr1);

	FAIL_IF(addr1[0] != '\0', "kzfree didn't zero the memory");

	return ret;
}

int ff_mm_6()
{
	int ret = 0;
	struct mm_stats stats;

	struct mm_superblock *superblock = create_superblock(test_heap_start_addr1,
							     test_heap_start_addr1,
							     test_heap_end_addr1);

	char *addr1 = (char *) kzalloc(superblock, sizeof(char), FRAME_BLOCK_SIZE*2);
	FAIL_IF(addr1 == NULL, "couldn't allocation block FRAME_BLOCK_SIZE*2");

	kzfree(superblock, addr1);

	char *addr2 = (char *) kzalloc(superblock, sizeof(char), FRAME_BLOCK_SIZE*2);
	FAIL_IF(addr1 != addr2, "should return the same block after freeing");

	stats = get_mm_stats(superblock, &stats);
	FAIL_IF(stats.bytes_used != (FRAME_BLOCK_SIZE*2), "incorrect bytes_used");
	FAIL_IF(stats.total_bytes != 2048, "incorrect total_bytes");

	return ret;
}

int ff_mm_7()
{
	int ret = 0;

	struct mm_superblock *superblock = create_superblock(test_heap_start_addr1,
							     test_heap_start_addr1,
							     test_heap_end_addr1);
	create_superblock((size_t) superblock, test_heap_start_addr2, test_heap_end_addr2);

	char *addr1 = (char *) kzalloc(superblock, sizeof(char), 2048);
	FAIL_IF(addr1 == NULL, "couldn't allocate maximum available in segment 1");

	kzfree(superblock, addr1);

	char *addr2 = (char *) kzalloc(superblock, sizeof(char), 2048);
	FAIL_IF(addr2 == NULL, "couldn't allocate maximum available in segment 2");
	FAIL_IF(addr2 != addr1, "should return the same frame as addr1");

	char *addr3 = (char *) kzalloc(superblock, sizeof(char), 2048+1);
	FAIL_IF(addr3 != NULL, "should have run out of memory");

	return ret;
}

#endif
