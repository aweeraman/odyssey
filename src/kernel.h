/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

#define STRINGIFY2(ver) #ver
#define STRINGIFY(ver) STRINGIFY2(ver)
#define CONFIG_VERSION STRINGIFY(CONFIG_VERSION_MAJOR) "." \
  STRINGIFY(CONFIG_VERSION_MINOR)

struct acpi_descriptor_v1 {
        char     signature[8];
        uint8_t  checksum;
        char     oem_id[6];
        uint8_t  revision;
        uint32_t rsdt_addr;
} __attribute__((packed));

struct acpi_descriptor_v2 {
        char     signature[8];
        uint8_t  checksum;
        char     oem_id[6];
        uint8_t  revision;
        uint32_t rsdt_addr;
        uint32_t length;
        uint64_t xsdt_addr;
        uint8_t  xchecksum;
        uint8_t  reserved[3];
} __attribute__((packed));

struct boot_device {
        uint32_t biosdev;
        uint32_t partition;
        uint32_t sub_partition;
} __attribute__((packed));

#endif
