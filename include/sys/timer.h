/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define MAX_N_TICKS_TIMERS 5

struct n_ticks_timer {
	int n_ticks;
	void (*callback)();
};

void tick();
uint32_t get_loops_per_tick();
int register_n_ticks_timer(int n_ticks, void (*timer_callback)());
void busy_wait(int ticks_to_wait);

#endif
