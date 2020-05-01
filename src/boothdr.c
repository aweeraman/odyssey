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
#include "kernel.h"
#include "tty.h"

static char boot_cmdline[BOOT_CMDLINE_MAX];

extern struct acpi_descriptor_v1        *acpi_v1;
extern struct acpi_descriptor_v2        *acpi_v2;
extern struct boot_device               *boot_dev;
extern struct multiboot_tag_framebuffer *framebuffer;

/*
 * Extract video/framebuffer details first to initialize console for output
 */
void early_framebuffer_console_init(size_t magic, size_t addr)
{
        struct multiboot_tag *tag;

        // Check if bootloader complies with multiboot2
        if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
                panic("Please use a bootloader that supports the Multiboot2 "
                                "specification.");
        }

        for (tag = (struct multiboot_tag *) ((size_t) (addr + 8));
                        tag->type != MULTIBOOT_TAG_TYPE_END;
                        tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag +
                                      ((tag->size + 7) & ~7))) {

        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
                framebuffer = (struct multiboot_tag_framebuffer *) tag;

                init_console();

                printf("Video addr=0x%X pitch=%d width=%d height=%d bpp=%d type=%d\n",
                  framebuffer->common.framebuffer_addr,
                  framebuffer->common.framebuffer_pitch,
                  framebuffer->common.framebuffer_width,
                  framebuffer->common.framebuffer_height,
                  framebuffer->common.framebuffer_bpp,
                  framebuffer->common.framebuffer_type);
                }
        }
}

/*
 * Extract multiboot provided information
 */
void read_multiboot_header_tags(size_t magic, size_t addr)
{
        int counter;
        multiboot_memory_map_t *mmap;
        struct multiboot_tag *tag;
        struct multiboot_tag_basic_meminfo *meminfo;
        struct multiboot_tag_bootdev *bootdev_tag;

        // Check if bootloader complies with multiboot2
        if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
                panic("Please use a bootloader that supports the Multiboot2"
                                "specification.");
        }

        printf("Multiboot information structure: start=0x%x %uB\n",
                        addr, *(size_t *) addr);

        for (tag = (struct multiboot_tag *) ((size_t) (addr + 8));
                        tag->type != MULTIBOOT_TAG_TYPE_END;
                        tag = (struct multiboot_tag *) ((multiboot_uint8_t *)
                                tag + ((tag->size + 7) & ~7))) {

                switch (tag->type) {

                        case MULTIBOOT_TAG_TYPE_CMDLINE:
                        printf("Command line: %s\n",
                                ((struct multiboot_tag_string *) tag)->string);
                        break;

                        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                        strncpy(boot_cmdline,
                                ((struct multiboot_tag_string *) tag)->string,
                                BOOT_CMDLINE_MAX-1);
                        printf("Boot loader: %s\n", boot_cmdline);
                        break;

                        case MULTIBOOT_TAG_TYPE_MODULE:
                        printf("Module: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                        meminfo = (struct multiboot_tag_basic_meminfo *) tag;
                        set_basic_meminfo(meminfo->mem_lower, meminfo->mem_upper);
                        printf("Basic memory info: lower=%dkB upper=%dkB\n",
                                        meminfo->mem_lower,
                                        meminfo->mem_upper);
                        break;

                        case MULTIBOOT_TAG_TYPE_BOOTDEV:
                        bootdev_tag = (struct multiboot_tag_bootdev *) tag;
                        boot_dev->biosdev = bootdev_tag->biosdev;
                        boot_dev->partition = bootdev_tag->slice;
                        boot_dev->sub_partition = bootdev_tag->part;

                        printf("Boot device: dev=0x%x slice=0x%x part=0x%x\n",
                            boot_dev->biosdev,
                            boot_dev->partition,
                            boot_dev->sub_partition);

                        break;

                        case MULTIBOOT_TAG_TYPE_MMAP:
                        for (counter = 0, mmap = ((struct multiboot_tag_mmap *) tag)->entries;
                             (multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
                             counter++, mmap = (multiboot_memory_map_t *)
                                   ((unsigned long) mmap +
                                    ((struct multiboot_tag_mmap *) tag)->entry_size)) {

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
                        break;

                        case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
                        printf("ELF sections: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_APM:
                        printf("APM: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI32:
                        printf("EFI 32-bit system table pointer: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI64:
                        printf("EFI 64-bit system table pointer: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_SMBIOS:
                        printf("SMBIOS: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_ACPI_OLD:
                        acpi_v1 = (struct acpi_descriptor_v1 *)
                                        ((struct multiboot_tag_old_acpi *) tag)->rsdp;

                        acpi_v2->oem_id[5] = '\0';

                        printf("ACPI v1 RSDP: rev=%d rsdt_addr=0x%x oem=%s\n",
                            acpi_v1->revision,
                            acpi_v1->rsdt_addr,
                            acpi_v1->oem_id);

                        break;

                        case MULTIBOOT_TAG_TYPE_ACPI_NEW:
                        acpi_v2 = (struct acpi_descriptor_v2 *)
                                        ((struct multiboot_tag_new_acpi *) tag)->rsdp;

                        acpi_v2->oem_id[5] = '\0';

                        printf("ACPI v2 RSDP: rev=%d rsdt_addr=0x%x xsdt_addr=0x%X oem=%s\n",
                            acpi_v2->revision,
                            acpi_v2->rsdt_addr,
                            acpi_v2->xsdt_addr,
                            acpi_v2->oem_id);
                        break;

                        case MULTIBOOT_TAG_TYPE_NETWORK:
                        printf("Network: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI_MMAP:
                        printf("EFI memory map: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI_BS:
                        printf("EFI boot services not terminated: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI32_IH:
                        printf("EFI 32-bit image handle pointer: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI64_IH:
                        printf("EFI 64-bit image handle pointer: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
                        printf("Image load base address: 0x%x\n",
                                ((struct multiboot_tag_load_base_addr *) tag)->load_base_addr);
                        break;
                }
        }
}
