/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stddef.h>
#include "error.h"

#define MAX_N_TICKS_TIMERS 5

typedef struct n_ticks_timer {
        int n_ticks;
        void (*callback)();
} n_ticks_timer_t;

void tick();
int  register_n_ticks_timer(int n_ticks, void (*timer_callback)());

#endif
