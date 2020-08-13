/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

ENTRY(_start)

MEMORY
{
    ram : ORIGIN = 0x82000000, LENGTH = 256K
}
 
SECTIONS
{
	. = 0x82000000;
	kernel_begin = .;
	.text :
	{
		*(.boot)
		*(.text)
	} > ram
	.rodata :
	{
		*(.rodata)
	} > ram
	.data :
	{
		*(.data)
	} > ram
	.bss :
	{
		*(.bss)
	} > ram
	kernel_end = .;
}
