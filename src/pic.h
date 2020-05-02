/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2020, Anuradha Weeraman
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
