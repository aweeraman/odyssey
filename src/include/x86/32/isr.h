/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef ISR_H
#define ISR_H

#include <stdint.h>

extern int irq0();
extern int irq1();
extern int irq2();
extern int irq3();
extern int irq4();
extern int irq5();
extern int irq6();
extern int irq7();
extern int irq8();
extern int irq9();
extern int irq10();
extern int irq11();
extern int irq12();
extern int irq13();
extern int irq14();
extern int irq15();

uint32_t irq0_address;
uint32_t irq1_address;
uint32_t irq2_address;
uint32_t irq3_address;
uint32_t irq4_address;
uint32_t irq5_address;
uint32_t irq6_address;
uint32_t irq7_address;
uint32_t irq8_address;
uint32_t irq9_address;
uint32_t irq10_address;
uint32_t irq11_address;
uint32_t irq12_address;
uint32_t irq13_address;
uint32_t irq14_address;
uint32_t irq15_address;

void irq0_handler();
void irq1_handler();
void irq2_handler();
void irq3_handler();
void irq4_handler();
void irq5_handler();
void irq6_handler();
void irq7_handler();
void irq8_handler();
void irq9_handler();
void irq10_handler();
void irq11_handler();
void irq12_handler();
void irq13_handler();
void irq14_handler();
void irq15_handler();

#endif
