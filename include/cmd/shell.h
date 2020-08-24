/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SHELL_H
#define SHELL_H

#include <lib/stdio.h>

#define MAX_CMD_LENGTH 256

struct kcmd {
	const char *cmd;
	int (*func)();
};

int cmd_modules();
int cmd_canary();
int cmd_clear();
int cmd_splash();
int cmd_exit();
int cmd_trigger_exception();
int cmd_help();
int cmd_osver();
int cmd_ret();
int cmd_memmap();
void start_interactive_shell();

#endif
