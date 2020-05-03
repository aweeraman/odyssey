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

#include "shell.h"
#include "keyboard.h"

char line[MAX_READLINE_LENGTH];

void start_interactive_shell()
{
        printf("\nMinos version %s\n", CONFIG_VERSION);

#if CONFIG_DRV_KEYBOARD
        for (;;) {
                printf("# ");
                block_and_readline(line);
        }
#else
        printf("Enable keyboard driver for interactive shell\n");
        for (;;) {
                asm("hlt");
        }
#endif
}
