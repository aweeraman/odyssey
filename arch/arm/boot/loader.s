/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

        .global _start

        .section .boot
_start:
        ldr sp, =stack_top
        bl kernel_main

hang:   b hang

        .section .bss
stack_bottom:
        .skip 4096
stack_top:
