/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TEST_H
#define TEST_H

#define TEST(test) \
	extern int test(); \
	run(test)

#define FAIL_IF(expr, msg) \
	if (expr) { \
		fail(__FUNCTION__, __LINE__, msg); \
		ret = 1; \
	} \

#define FAIL_AND_EXIT_IF(expr, msg) \
	if (expr) { \
		fail(__FUNCTION__, __LINE__, msg); \
		return 1; \
	} \

void run_tests();
void fail(const char *test, const int line, char *msg);

#endif
