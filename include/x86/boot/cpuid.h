/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CPUID_H
#define CPUID_H

#include <stddef.h>

extern int cpu_has_fpu, cpu_has_vme;

size_t *cpuid();
void get_cpu_features();

#endif
