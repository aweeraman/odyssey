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
 *
 * This program includes code that is Copyright (C) 1999, 2010 Free
 * Software Foundation, Inc.
 */

#include "kernel.h"
#include "multiboot2.h"
#include "tty.h"
#include "string.h"

void kernel_main(__uint64_t magic, __uint64_t addr) {
  struct multiboot_tag *tag;
  multiboot_memory_map_t *mmap;

  clear();

  // Initialize serial port for communication
  init_serial();
  println("Initialized serial: COM1");

  // Check if bootloader complies with multiboot2
  if (magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
    printf("Multiboot2 header: 0x%x [valid]\n", magic);
  }

  printf("Multiboot2 structure size: 0x%x\n", *(size_t *) addr);

  for (tag = (struct multiboot_tag *) (addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag +
                                      ((tag->size + 7) & ~7))) {
    switch (tag->type) {
      case MULTIBOOT_TAG_TYPE_CMDLINE:
        printf("  CMDLINE, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        printf("  BOOT_LOADER: %s\n",
            ((struct multiboot_tag_string *) tag)->string);
        break;

      case MULTIBOOT_TAG_TYPE_MODULE:
        printf("  MODULE, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        printf("  BASIC_MEMINFO, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_BOOTDEV:
        printf("  BOOTDEV, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_MMAP:
        printf("  MMAP, size 0x%x\n", tag->size);
        int counter = 0;
        for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
            (multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
            mmap = (multiboot_memory_map_t *)
                   ((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag)->entry_size)) {
          printf("    %d: 0x%x%x - 0x%x%x (%d) [%d, %s]\n",
              counter++,
              (size_t) (mmap->addr >> 32),
              (size_t) (mmap->addr & 0xffffffff),
              (size_t) (mmap->len >> 32),
              (size_t) (mmap->len & 0xffffffff),
              (__uint64_t) mmap->len,
              (size_t) (mmap->type),
              MEMORY_REGION_T[(size_t) (mmap->type)]);
        }
        break;

      case MULTIBOOT_TAG_TYPE_VBE:
        printf("  VBE, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        printf("  FRAMEBUFFER, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
        printf("  ELF_SECTIONS, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_APM:
        printf("  APM, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI32:
        printf("  EFI32, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI64:
        printf("  EFI64, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_SMBIOS:
        printf("  SMBIOS, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_ACPI_OLD:
        printf("  ACPI_OLD, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_ACPI_NEW:
        printf("  ACPI_NEW, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_NETWORK:
        printf("  NETWORK, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI_MMAP:
        printf("  EFI_MMAP, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI_BS:
        printf("  EFI_BS, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI32_IH:
        printf("  EFI32_IH, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI64_IH:
        printf("  EFI64_IH, size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
        printf("  LOAD_BASE_ADDR, size 0x%x\n", tag->size);
        break;
    }
  }

  println("Boot complete, exiting kernel");
}
