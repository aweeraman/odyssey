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

#include "boothdr.h"
#include "memory.h"
#include "libk.h"

static char boot_cmdline[BOOT_CMDLINE_MAX];

/*
 * Extract multiboot provided information
 */
void init_mb(size_t magic, size_t addr) {
  int counter;
  multiboot_memory_map_t *mmap;
  struct multiboot_tag *tag;
  struct multiboot_tag_basic_meminfo *meminfo;

  // Check if bootloader complies with multiboot2
  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    panic("Please use a bootloader that supports the Multiboot2 specification.");
  }

  for (tag = (struct multiboot_tag *) ((size_t) (addr + 8));
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag +
                                      ((tag->size + 7) & ~7))) {
    switch (tag->type) {
      case MULTIBOOT_TAG_TYPE_CMDLINE:
        printf("BOOT ARGUMENTS: %s\n", ((struct multiboot_tag_string *) tag)->string);
        break;

      case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        strncpy(boot_cmdline, ((struct multiboot_tag_string *) tag)->string, BOOT_CMDLINE_MAX);
        printf("BOOT LOADER: %s\n", boot_cmdline);
        break;

      case MULTIBOOT_TAG_TYPE_MODULE:
        printf("MODULE: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        meminfo = (struct multiboot_tag_basic_meminfo *) tag;
        set_basic_meminfo(meminfo->mem_lower, meminfo->mem_upper);
        printf("BASIC MEMORY INFO: lower=%dkB, upper=%dkB\n", meminfo->mem_lower, meminfo->mem_upper);
        break;

      case MULTIBOOT_TAG_TYPE_BOOTDEV:
        printf("BOOT DEVICE: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_MMAP:
        for (counter = 0, mmap = ((struct multiboot_tag_mmap *) tag)->entries;
            (multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
            counter++, mmap = (multiboot_memory_map_t *)
                   ((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag)->entry_size)) {
                add_mem_region(counter,
                               (size_t) mmap->addr,
                               (size_t) mmap->len,
                               (size_t) mmap->type);
                set_num_mem_regions(counter);
        }
        print_mem_regions();
        break;

      case MULTIBOOT_TAG_TYPE_VBE:
        printf("VBE: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        printf("FRAMEBUFFER: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
        printf("ELF SECTIONS: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_APM:
        printf("APM: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI32:
        printf("EFI32: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI64:
        printf("EFI64: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_SMBIOS:
        printf("SMBIOS: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_ACPI_OLD:
        printf("ACPI OLD: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_ACPI_NEW:
        printf("ACPI NEW: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_NETWORK:
        printf("NETWORK: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI_MMAP:
        printf("EFI MMAP: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI_BS:
        printf("EFI BS: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI32_IH:
        printf("EFI32 IH: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_EFI64_IH:
        printf("EFI64 IH: size 0x%x\n", tag->size);
        break;

      case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
        printf("LOAD BASE ADDRESS: 0x%x\n",
            ((struct multiboot_tag_load_base_addr *) tag)->load_base_addr);
        break;
    }
  }
}
