/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/io.h>
#include <sys/timer.h>

extern volatile uint32_t ticks;

void outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void outw(uint16_t port, uint16_t val)
{
	asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

uint16_t inw(uint16_t port)
{
	uint16_t ret;
	asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

void io_wait()
{
	uint32_t end = ticks + 2;
	while (ticks < end);
}
