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

extern int exception0();
extern int exception1();
extern int exception2();
extern int exception3();
extern int exception4();
extern int exception5();
extern int exception6();
extern int exception7();
extern int exception8();
extern int exception9();
extern int exception10();
extern int exception11();
extern int exception12();
extern int exception13();
extern int exception14();
extern int exception15();
extern int exception16();
extern int exception17();
extern int exception18();
extern int exception19();
extern int exception20();
extern int exception21();
extern int exception22();
extern int exception23();
extern int exception24();
extern int exception25();
extern int exception26();
extern int exception27();
extern int exception28();
extern int exception29();
extern int exception30();
extern int exception31();

void exception_handler();

#endif
