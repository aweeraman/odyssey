/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <test.h>
#include <lib/string.h>

int memcpy_1()
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

int memset_1()
{
        int ret = 0;
        char str[3];
        void *p;

        p = memset(str, 'a', sizeof(str));
        ASSERT(!(str[0] == 'a' && str[1] == 'a' && str[2] == 'a'),
                        "all elements are not updated correctly");
        ASSERT(p != str, "invalid pointer returned");

        return ret;
}

int strnlen_1()
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

int strncpy_1()
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

int strncmp_1()
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
