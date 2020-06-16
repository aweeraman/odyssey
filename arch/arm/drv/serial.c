/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef CONFIG_SERIAL

#include <stdint.h>
#include <sys/serial.h>

#define UART0_BASE 0x1c090000

void write_serial(unsigned char a)
{
	*(volatile uint32_t *)(UART0_BASE) = (uint32_t) a;
}

#endif
