/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <test.h>
#include <lib/stdio.h>

static int tests_passed;
static int tests_failed;

void fail(const char *test, const int line, char *msg)
{
        printk("Test %s failed: line %d, %s\n", test, line, msg);
}

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
        TEST(multiboot2_magic_1);
#endif

#if CONFIG_MM_FF
        TEST(ff_mm_1);
        TEST(ff_mm_2);
        TEST(ff_mm_3);
        TEST(ff_mm_4);
#endif

        printk("Tests run, %d / %d passed\n", tests_passed, tests_passed+tests_failed);
}
