/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <test.h>
#include <lib/termio.h>
#include <lib/string.h>
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

static int memcpy_1()
{
        int ret = 0;
        char str1[5];
        char *str2 = "123";
        char *str3 = "12345";
        char str4[5] = { '1', '2', '\0', '3', '4' };
        char str5[5] = { 'p', 'p', 'p', 'p', 'p' };
        void *p;

        p = memcpy(str1, str2, 4);
        ASSERT(strnlen(str1, 5) != 3, "string length doesn't match, expected 3");
        ASSERT(p != str1, "invalid pointer returned");

        memcpy(str1, str3, 5);
        ASSERT(!(str1[0] == '1' && str1[1] == '2' && str1[2] == '3' &&
                 str1[3] == '4' && str1[4] == '5'), "string is not identical");

        memcpy(str5, str4, 5);
        ASSERT(!(str5[0] == '1' && str5[1] == '2' && str5[2] == '\0' &&
                 str5[3] == '3' && str5[4] == '4'),
                 "string is not identical, interleaving null");

        return ret;
}

static int memset_1()
{
        int ret = 0;
        char str[3];
        void *p;

        p = memset(str, 'a', sizeof(str));
        ASSERT(!(str[0] == 'a' && str[1] == 'a' && str[2] == 'a'), "all elements are not updated correctly");
        ASSERT(p != str, "invalid pointer returned");

        return ret;
}

static int strnlen_1()
{
        int ret = 0;
        char *str1 = "123";
        char *str2 = "12345";
        char *str3 = "123456";

        ASSERT(strnlen(str1, 5) != 3, "string length doesn't match, expected 3");
        ASSERT(strnlen(str2, 5) != 5, "string length doesn't match, expected 5");
        ASSERT(strnlen(str3, 5) != 5, "string length doesn't match, expected 5, not 6");

        return ret;
}

static int strncpy_1()
{
        int ret = 0;
        char str1[5];
        char *str2 = "123";
        char *str3 = "12345";
        char str4[5] = { '1', '2', '\0', '3' };
        char str5[5] = { 'p', 'p', 'p', 'p', 'p' };
        void *p;

        p = strncpy(str1, str2, 5);
        ASSERT(strnlen(str1, 5) != 3, "string length doesn't match, expected 3");
        ASSERT(p != str1, "invalid pointer returned");

        strncpy(str1, str2, 5);
        ASSERT(!(str1[0] == '1' && str1[1] == '2' &&
                 str1[2] == '3' && str1[3] == '\0'), "string is not identical/null terminated");

        strncpy(str1, str3, 5);
        ASSERT(!(str1[0] == '1' && str1[1] == '2' && str1[2] == '3' &&
                 str1[3] == '4' && str1[4] == '5'), "string is not identical");

        strncpy(str5, str4, 5);
        ASSERT(!(str5[0] == '1' && str5[1] == '2' && str5[2] == '\0' &&
                 str5[3] == '\0' && str5[4] == '\0'), "copy doesn't honor null");

        return ret;
}

static int strncmp_1()
{
        int ret = 0;
        char *str1 = "123";
        char *str2 = "123";
        char *str3 = "124";
        char *str4 = "123456";
        char *str5 = "123457";

        ASSERT(strncmp(str1, str2, 5) != 0, "strings are not identical");
        ASSERT(strncmp(str1, str3, 5) >= 0, "str1 should be lesser");
        ASSERT(strncmp(str3, str1, 5) <= 0, "str3 should be greater");
        ASSERT(strncmp(str4, str5, 5) != 0, "out of bounds strings are not identical");

        return ret;
}

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

        run(strnlen_1);
        run(strncpy_1);
        run(strncmp_1);
        run(memset_1);
        run(memcpy_1);

#ifdef ARCH_X86
        run(multiboot2_magic_1);
#endif

#ifdef CONFIG_MM_FF
        run(ff_mm_1);
#endif

        printf("Tests run, %d / %d passed\n", tests_passed, tests_passed+tests_failed);
}
