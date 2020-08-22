/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/boot/cpuid.h>
#include <lib/stdio.h>

static size_t cpuid_str[4];
static size_t highest_functionality, cpu_feature_flags_ecx, cpu_feature_flags_edx;

// Define the CPU features when EAX is 1
int CPU_FEATURES_1;

static const char *cpu_features_ecx[32] = {
	"sse3", "pclmulqdq", "dtes64", "monitor", "ds-cpl", "vmx",
	"smx", "est", "tm2", "ssse3", "cnxt-id", "sdbg", "fma", "cx16",
	"xtpr", "pdcm", "reserved", "pcid", "dca", "sse4.1", "sse4.2",
	"x2apic", "movbe", "popcnt", "tsc-deadline", "aes", "xsave",
	"osxsave", "avx", "f16c", "rdrnd", "hypervisor"
};

static const char *cpu_features_edx[32] = {
	"fpu", "vme", "de", "pse", "tsc", "msr", "pae", "mce", "cx8",
	"apic", "reserved", "sep", "mtrr", "pge", "mca", "cmov", "pat",
	"pse-36", "psn", "clfsh", "reserved", "ds", "acpi", "mmx",
	"fxsr", "sse", "sse2", "ss", "htt", "tm", "ia64", "pbe"
};
//------ EAX 1

void get_cpu_features() {
	asm("mov $0x1, %eax");
	asm("cpuid");
	asm("mov %%ecx, %0" :"=r" (cpu_feature_flags_ecx));
	asm("mov %%edx, %0" :"=r" (cpu_feature_flags_edx));

	printk("  ");
	for (int i = 0; i < 32; i++) {
		if ((cpu_feature_flags_ecx >> i) & 0x1)
			printk("%s ", cpu_features_ecx[i]);
		if ((cpu_feature_flags_edx >> i) & 0x1)
			printk("%s ", cpu_features_edx[i]);
	}
	printk("\n");

	cpu_has_fpu    = (cpu_feature_flags_edx >> 0) & 0x1;
	cpu_has_vme    = (cpu_feature_flags_edx >> 1) & 0x1;
	cpu_has_de     = (cpu_feature_flags_edx >> 2) & 0x1;
	cpu_has_pse    = (cpu_feature_flags_edx >> 3) & 0x1;
	cpu_has_tsc    = (cpu_feature_flags_edx >> 4) & 0x1;
	cpu_has_msr    = (cpu_feature_flags_edx >> 5) & 0x1;
	cpu_has_pae    = (cpu_feature_flags_edx >> 6) & 0x1;
	cpu_has_mce    = (cpu_feature_flags_edx >> 7) & 0x1;
	cpu_has_cx8    = (cpu_feature_flags_edx >> 8) & 0x1;
	cpu_has_apic   = (cpu_feature_flags_edx >> 9) & 0x1;
	// reserved
	cpu_has_sep    = (cpu_feature_flags_edx >> 11) & 0x1;
	cpu_has_mtrr   = (cpu_feature_flags_edx >> 12) & 0x1;
	cpu_has_pge    = (cpu_feature_flags_edx >> 13) & 0x1;
	cpu_has_mca    = (cpu_feature_flags_edx >> 14) & 0x1;
	cpu_has_cmov   = (cpu_feature_flags_edx >> 15) & 0x1;
	cpu_has_pat    = (cpu_feature_flags_edx >> 16) & 0x1;
	cpu_has_pse_36 = (cpu_feature_flags_edx >> 17) & 0x1;
	cpu_has_psn    = (cpu_feature_flags_edx >> 18) & 0x1;
	cpu_has_clfsh  = (cpu_feature_flags_edx >> 19) & 0x1;
	//reserved
	cpu_has_ds     = (cpu_feature_flags_edx >> 21) & 0x1;
	cpu_has_acpi   = (cpu_feature_flags_edx >> 22) & 0x1;
	cpu_has_mmx    = (cpu_feature_flags_edx >> 23) & 0x1;
	cpu_has_fxsr   = (cpu_feature_flags_edx >> 24) & 0x1;
	cpu_has_sse    = (cpu_feature_flags_edx >> 25) & 0x1;
	cpu_has_sse2   = (cpu_feature_flags_edx >> 26) & 0x1;
	cpu_has_ss     = (cpu_feature_flags_edx >> 27) & 0x1;
	cpu_has_htt    = (cpu_feature_flags_edx >> 28) & 0x1;
	cpu_has_tm     = (cpu_feature_flags_edx >> 29) & 0x1;
	cpu_has_ia64   = (cpu_feature_flags_edx >> 30) & 0x1;
	cpu_has_pbe    = (cpu_feature_flags_edx >> 31) & 0x1;
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

	get_cpu_features();

	return cpuid_str;
}
