/**
 * SPDX-License-Identifier: GPL-3.0-or-later
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
