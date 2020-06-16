/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BOOTDEV_H
#define BOOTDEV_H

struct boot_device {
	uint32_t biosdev;
	uint32_t partition;
	uint32_t sub_partition;
} __attribute__((packed));

#endif
