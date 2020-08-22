/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/boot/cpuid.h>
#include <lib/stdio.h>

static size_t cpuid_str[4];
static size_t highest_functionality, cpu_feature_flags_ecx, cpu_feature_flags_edx;

// Define the CPU features when EAX is 1
int CPU_FEATURES;

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

	cpu_has_sse3         = (cpu_feature_flags_ecx >> 0)  & 0x1;
	cpu_has_pclmulqdq    = (cpu_feature_flags_ecx >> 1)  & 0x1;
	cpu_has_dtes64       = (cpu_feature_flags_ecx >> 2)  & 0x1;
	cpu_has_monitor      = (cpu_feature_flags_ecx >> 3)  & 0x1;
	cpu_has_ds_cpl       = (cpu_feature_flags_ecx >> 4)  & 0x1;
	cpu_has_vmx          = (cpu_feature_flags_ecx >> 5)  & 0x1;
	cpu_has_smx          = (cpu_feature_flags_ecx >> 6)  & 0x1;
	cpu_has_est          = (cpu_feature_flags_ecx >> 7)  & 0x1;
	cpu_has_tm2          = (cpu_feature_flags_ecx >> 8)  & 0x1;
	cpu_has_ssse3        = (cpu_feature_flags_ecx >> 9)  & 0x1;
	cpu_has_cnxt_id      = (cpu_feature_flags_ecx >> 10) & 0x1;
	cpu_has_sdbg         = (cpu_feature_flags_ecx >> 11) & 0x1;
	cpu_has_fma          = (cpu_feature_flags_ecx >> 12) & 0x1;
	cpu_has_cx16         = (cpu_feature_flags_ecx >> 13) & 0x1;
	cpu_has_xtpr         = (cpu_feature_flags_ecx >> 14) & 0x1;
	cpu_has_pdcm         = (cpu_feature_flags_ecx >> 15) & 0x1;
	//reserved
	cpu_has_pcid         = (cpu_feature_flags_ecx >> 17) & 0x1;
	cpu_has_dca          = (cpu_feature_flags_ecx >> 18) & 0x1;
	cpu_has_sse4_1       = (cpu_feature_flags_ecx >> 19) & 0x1;
	cpu_has_sse4_2       = (cpu_feature_flags_ecx >> 20) & 0x1;
	cpu_has_x2apic       = (cpu_feature_flags_ecx >> 21) & 0x1;
	cpu_has_movbe        = (cpu_feature_flags_ecx >> 22) & 0x1;
	cpu_has_popcnt       = (cpu_feature_flags_ecx >> 23) & 0x1;
	cpu_has_tsc_deadline = (cpu_feature_flags_ecx >> 24) & 0x1;
	cpu_has_aes          = (cpu_feature_flags_ecx >> 25) & 0x1;
	cpu_has_xsave        = (cpu_feature_flags_ecx >> 26) & 0x1;
	cpu_has_osxsave      = (cpu_feature_flags_ecx >> 27) & 0x1;
	cpu_has_avx          = (cpu_feature_flags_ecx >> 28) & 0x1;
	cpu_has_f16c         = (cpu_feature_flags_ecx >> 29) & 0x1;
	cpu_has_rdrnd        = (cpu_feature_flags_ecx >> 30) & 0x1;
	cpu_has_hypervisor   = (cpu_feature_flags_ecx >> 31) & 0x1;

	cpu_has_fpu    = (cpu_feature_flags_edx >> 0)  & 0x1;
	cpu_has_vme    = (cpu_feature_flags_edx >> 1)  & 0x1;
	cpu_has_de     = (cpu_feature_flags_edx >> 2)  & 0x1;
	cpu_has_pse    = (cpu_feature_flags_edx >> 3)  & 0x1;
	cpu_has_tsc    = (cpu_feature_flags_edx >> 4)  & 0x1;
	cpu_has_msr    = (cpu_feature_flags_edx >> 5)  & 0x1;
	cpu_has_pae    = (cpu_feature_flags_edx >> 6)  & 0x1;
	cpu_has_mce    = (cpu_feature_flags_edx >> 7)  & 0x1;
	cpu_has_cx8    = (cpu_feature_flags_edx >> 8)  & 0x1;
	cpu_has_apic   = (cpu_feature_flags_edx >> 9)  & 0x1;
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
