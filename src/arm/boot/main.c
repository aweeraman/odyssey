/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdint.h>

#define UART0_BASE 0x1c090000
 
void kernel_main() {
    *(volatile uint32_t *)(UART0_BASE) = 'O';
    *(volatile uint32_t *)(UART0_BASE) = 'd';
}
