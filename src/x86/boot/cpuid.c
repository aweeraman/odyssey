/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cpuid.h>
#include <stdint.h>
#include <lib/stdio.h>

static uint32_t cpuid_str[4];
static uint32_t highest_functionality;

uint32_t *cpuid()
{
	asm("mov $0x0, %eax");
	asm("cpuid");
	asm("mov %%eax, %0"
			:"=r" (highest_functionality));
	asm("mov %%ebx, %0"
			:"=r" (cpuid_str[0]));
	asm("mov %%edx, %0"
			:"=r" (cpuid_str[1]));
	asm("mov %%ecx, %0"
			:"=r" (cpuid_str[2]));
	cpuid_str[3] = '\0';

	printk("Processor vendor ID is %s, highest function 0x%x\n", cpuid_str, highest_functionality);

	return cpuid_str;
}
