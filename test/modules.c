/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#if ARCH_X86

#include <test.h>
#include <x86/boot/modules.h>

int canary_1()
{
	int ret = 0;
	uint32_t tweet = 0;

	module_t canary = (module_t) get_module_by_idx(0);
	FAIL_AND_EXIT_IF(canary == 0, "No boot module returned for canary");

	canary();
	asm("\t movl %%eax, %0" : "=r"(tweet));
	FAIL_IF(tweet != CANARY_MAGIC_STRING, "Canary didn't say the magic word");

	return ret;
}

#endif
