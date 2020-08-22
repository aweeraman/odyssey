/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CPUID_H
#define CPUID_H

#include <stddef.h>

#define CPU_FEATURES cpu_has_fpu, cpu_has_vme, cpu_has_de, cpu_has_pse, \
	   cpu_has_tsc, cpu_has_msr, cpu_has_pae, cpu_has_mce, cpu_has_cx8, \
	   cpu_has_apic, cpu_has_sep, cpu_has_mtrr, cpu_has_pge, \
	   cpu_has_mca, cpu_has_cmov, cpu_has_pat, cpu_has_pse_36, cpu_has_psn, \
	   cpu_has_clfsh, cpu_has_ds, cpu_has_acpi, cpu_has_mmx, cpu_has_fxsr, \
	   cpu_has_sse, cpu_has_sse2, cpu_has_ss, cpu_has_htt, cpu_has_tm, \
	   cpu_has_ia64, cpu_has_pbe, cpu_has_sse3, cpu_has_pclmulqdq, \
	   cpu_has_dtes64, cpu_has_monitor, cpu_has_ds_cpl, cpu_has_vmx, \
	   cpu_has_smx, cpu_has_est, cpu_has_tm2, cpu_has_ssse3, cpu_has_cnxt_id, \
	   cpu_has_sdbg, cpu_has_fma, cpu_has_cx16, cpu_has_xtpr, cpu_has_pdcm, \
	   cpu_has_pcid, cpu_has_dca, cpu_has_sse4_1, cpu_has_sse4_2, \
	   cpu_has_x2apic, cpu_has_movbe, cpu_has_popcnt, cpu_has_tsc_deadline, \
	   cpu_has_aes, cpu_has_xsave, cpu_has_osxsave, cpu_has_avx, \
	   cpu_has_f16c, cpu_has_rdrnd, cpu_has_hypervisor

extern int CPU_FEATURES;

size_t *cpuid();
void get_cpu_features();

#endif
