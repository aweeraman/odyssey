/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PIC_H
#define PIC_H

#define EOI                0x20
#define PIC_MASTER         0x20
#define PIC_SLAVE          0xA0
#define PIC_MASTER_COMMAND PIC_MASTER
#define PIC_MASTER_DATA    (PIC_MASTER+1)
#define PIC_SLAVE_COMMAND  PIC_SLAVE
#define PIC_SLAVE_DATA     (PIC_SLAVE+1)
#define PIC_MASK           1
#define PIC_UNMASK         0

void pic_init();
void pic_eoi(int irq);
void pic_mask(int irq, short mask);

#endif
