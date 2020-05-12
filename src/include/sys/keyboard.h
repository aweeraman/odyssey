/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define MAX_READLINE_LENGTH 256

void keyboard_init();
void kbd_interrupt();
uint8_t block_and_readline(char *line);

#endif
