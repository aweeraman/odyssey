/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TEST_H
#define TEST_H

#define ASSERT(expr, msg) \
        if (expr) { \
                fail(__FUNCTION__, __LINE__, msg); \
                ret = 1; \
        } \

void run_tests();
void fail(const char *test, const int line, char *msg);

#endif
