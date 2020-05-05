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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define MAX_READLINE_LENGTH 256
#define READLINE_READY      0
#define READLINE_BLOCKED    1

#define KBD_DATA_PORT       0x60
#define KBD_CMD_PORT        0x64

void keyboard_init();
void kbd_interrupt();
uint8_t block_and_readline(char *line);

#endif
