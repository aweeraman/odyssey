/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/boot/cpuid.h>
#include <lib/stdio.h>

static size_t cpuid_str[4];
static size_t highest_functionality, cpu_feature_flags_ecx, cpu_feature_flags_edx;
static char *cpu_features_ecx[32] = {
	"sse3", "pclmulqdq", "dtes64", "monitor", "ds-cpl", "vmx",
	"smx", "est", "tm2", "ssse3", "cnxt-id", "sdbg", "fma", "cx16",
	"xtpr", "pdcm", "reserved", "pcid", "dca", "sse4.1", "sse4.2",
	"x2apic", "movbe", "popcnt", "tsc-deadline", "aes", "xsave",
	"osxsave", "avx", "f16c", "rdrnd", "hypervisor"
};
static char *cpu_features_edx[32] = {
	"fpu", "vme", "de", "pse", "tsc", "msr", "pae", "mce", "cx8",
	"apic", "reserved", "sep", "mtrr", "pge", "mca", "cmov", "pat",
	"pse-36", "psn", "clfsh", "reserved", "ds", "acpi", "mmx",
	"fxsr", "sse", "sse2", "ss", "htt", "tm", "ia64", "pbe"
};

int cpu_has_fpu()
{
	return (cpu_feature_flags_edx >> 0) & 0x1;
}

int cpu_has_vme()
{
	return (cpu_feature_flags_edx >> 1) & 0x1;
}

size_t *cpuid()
{
	asm("mov $0x0, %eax");
	asm("cpuid");
	asm("mov %%eax, %0" :"=r" (highest_functionality));
	asm("mov %%ebx, %0" :"=r" (cpuid_str[0]));
	asm("mov %%edx, %0" :"=r" (cpuid_str[1]));
	asm("mov %%ecx, %0" :"=r" (cpuid_str[2]));
	cpuid_str[3] = '\0';

	printk("Processor vendor ID is %s, highest function 0x%x\n",
		cpuid_str, highest_functionality);

	asm("mov $0x1, %eax");
	asm("cpuid");
	asm("mov %%ecx, %0" :"=r" (cpu_feature_flags_ecx));
	asm("mov %%edx, %0" :"=r" (cpu_feature_flags_edx));

	printk("  Features: ");
	for (int i = 0; i < 32; i++) {
		if ((cpu_feature_flags_ecx >> i) & 0x1)
			printk("%s ", cpu_features_ecx[i]);
		if ((cpu_feature_flags_edx >> i) & 0x1)
			printk("%s ", cpu_features_edx[i]);
	}
	printk("\n");

	return cpuid_str;
}
