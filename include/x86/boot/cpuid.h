/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CPUID_H
#define CPUID_H

#include <stddef.h>

int cpu_has_fpu();
int cpu_has_vmu();
size_t *cpuid();

#endif
