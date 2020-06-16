/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/timer.h>
#include <lib/stdio.h>
#include <sys/tty.h>
#include <stddef.h>
#include <error.h>

uint32_t volatile ticks	= 0;
static uint32_t loops_per_tick = 0;
static int n_ticks_timer_idx   = 0;
static n_ticks_timer_t n_ticks_timers[MAX_N_TICKS_TIMERS];

void busy_wait(int ticks_to_wait)
{
	uint32_t start = ticks;
	uint32_t end   = ticks + ticks_to_wait;

	if (end < start)
		while (ticks > start);

	while (ticks < end);
}

uint32_t get_loops_per_tick()
{
	uint32_t loops = 0;
	uint32_t start_tick = ticks;

	for (loops = 0; start_tick == ticks; loops++);

	// Do it again, this one is more accurate
	start_tick = ticks;
	for (loops = 0; start_tick == ticks; loops++);

	return (loops_per_tick = loops);
}

void tick()
{
	ticks++;

	for (int i = 0; i < n_ticks_timer_idx; i++) {
		if (ticks % n_ticks_timers[i].n_ticks == 0)
			(n_ticks_timers[i].callback)();
	}
}

int register_n_ticks_timer(int n_ticks, void (*timer_callback)())
{
	if (n_ticks_timer_idx >= MAX_N_TICKS_TIMERS)
		return EMAXLIMIT;

	if (timer_callback == NULL)
		return ENULLPTR;

	n_ticks_timers[n_ticks_timer_idx++] = (n_ticks_timer_t) {
		.n_ticks  = n_ticks,
		.callback = timer_callback
	};

	return SUCCESS;
}
