/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2020, Anuradha Weeraman
 */

#include "timer.h"
#include "libk.h"
#include "tty.h"

uint32_t volatile ticks  = 0;
static int n_ticks_timer_idx = 0;
static n_ticks_timer_t n_ticks_timers[MAX_N_TICKS_TIMERS];

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
