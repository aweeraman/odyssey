/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SHELL_H
#define SHELL_H

#include <lib/stdio.h>

#define MAX_CMD_LENGTH 256

typedef struct cmd {
        const char *cmd;
        int (*func)();
} cmd_t;

int  cmd_clear();
int  cmd_splash();
int  cmd_exit();
int  cmd_trigger_exception();
int  cmd_help();
void start_interactive_shell();

#endif
