/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <test.h>
#include <lib/termio.h>
#include <sys/memory.h>

#ifdef CONFIG_MM_FF
#include <sys/ffmm.h>
#endif

static int tests_passed;
static int tests_failed;

void fail(const char *test, const int line, char *msg)
{
        printf("Test %s failed: line %d, %s\n", test, line, msg);
}

#ifdef CONFIG_MM_FF
static int ff_mm_1()
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

#ifdef ARCH_X86
static int multiboot2_magic_1()
{
        int ret = 0;
        mem_ptr_t *p = (mem_ptr_t *) 0x100000;

        ASSERT(p->word.w1 != 0x50d6, "peek at mem location 0x100000 is not 50d6");
        ASSERT(p->word.w2 != 0xe852, "peek at mem location 0x100002 is not e852");

        return ret;
}
#endif

static void run(int (*test)())
{
        int ret = (*test)();

        if (ret == 0)
                tests_passed++;
        else
                tests_failed++;
}

void run_tests()
{
        tests_passed = 0;
        tests_failed = 0;

        TEST(strnlen_1);
        TEST(strncpy_1);
        TEST(strncmp_1);
        TEST(memset_1);
        TEST(memcpy_1);

#ifdef ARCH_X86
        run(multiboot2_magic_1);
#endif

#ifdef CONFIG_MM_FF
        run(ff_mm_1);
#endif

        printf("Tests run, %d / %d passed\n", tests_passed, tests_passed+tests_failed);
}
