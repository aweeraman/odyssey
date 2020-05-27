/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/boot/boothdr.h>
#include <x86/boot/device.h>
#include <x86/32/acpi.h>
#include <mm/region.h>
#include <lib/stdio.h>
#include <lib/string.h>
#include <sys/panic.h>
#include <sys/tty.h>
#include <lib/mm.h>

static char boot_cmdline[BOOT_CMDLINE_MAX];

struct acpi_descriptor_v1        *acpi_v1;
struct acpi_descriptor_v2        *acpi_v2;
struct boot_device               *boot_dev;
struct multiboot_tag_framebuffer *framebuffer;

static size_t addr;

/*
 * Extract video/framebuffer details first to initialize console for output
 */
void early_framebuffer_console_init(size_t magic, size_t structure_addr)
{
        struct multiboot_tag *tag;

        // Check if bootloader complies with multiboot2
        if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
                panic("Please use a bootloader that supports Multiboot2.");
        }

        addr = structure_addr;

        for (tag = (struct multiboot_tag *) ((size_t) (addr + 8));
                        tag->type != MULTIBOOT_TAG_TYPE_END;
                        tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag +
                                      ((tag->size + 7) & ~7))) {

        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
                framebuffer = (struct multiboot_tag_framebuffer *) tag;

                init_console();

                printk("Video addr=0x%X pitch=%d width=%d height=%d bpp=%d type=%d\n",
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
void read_multiboot_header_tags()
{
        int counter;
        multiboot_memory_map_t *mmap;
        struct multiboot_tag *tag;
        struct multiboot_tag_basic_meminfo *meminfo;
        struct multiboot_tag_bootdev *bootdev_tag;

        printk("Multiboot information structure: start=0x%x %uB\n",
                        addr, *(size_t *) addr);

        for (tag = (struct multiboot_tag *) ((size_t) (addr + 8));
                        tag->type != MULTIBOOT_TAG_TYPE_END;
                        tag = (struct multiboot_tag *) ((multiboot_uint8_t *)
                                tag + ((tag->size + 7) & ~7))) {

                switch (tag->type) {

                        case MULTIBOOT_TAG_TYPE_CMDLINE:
                        printk("Command line: %s\n",
                                ((struct multiboot_tag_string *) tag)->string);
                        break;

                        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                        strncpy(boot_cmdline,
                                ((struct multiboot_tag_string *) tag)->string,
                                BOOT_CMDLINE_MAX-1);
                        printk("Boot loader: %s\n", boot_cmdline);
                        break;

                        case MULTIBOOT_TAG_TYPE_MODULE:
                        printk("Module: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                        meminfo = (struct multiboot_tag_basic_meminfo *) tag;
                        set_basic_meminfo(meminfo->mem_lower, meminfo->mem_upper);
                        printk("Basic memory info: lower=%dkB upper=%dkB\n",
                                        meminfo->mem_lower,
                                        meminfo->mem_upper);
                        break;

                        case MULTIBOOT_TAG_TYPE_BOOTDEV:
                        boot_dev = kalloc(NULL, sizeof(struct multiboot_tag_bootdev), 1);
                        if (boot_dev == NULL)
                                panic("OOM while allocating bootdev");

                        bootdev_tag = (struct multiboot_tag_bootdev *) tag;
                        boot_dev->biosdev = bootdev_tag->biosdev;
                        boot_dev->partition = bootdev_tag->slice;
                        boot_dev->sub_partition = bootdev_tag->part;

                        printk("Boot device: dev=0x%x slice=0x%x part=0x%x\n",
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
                        printk("VBE: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                        break;

                        case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
                        printk("ELF sections: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_APM:
                        printk("APM: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI32:
                        printk("EFI 32-bit system table pointer: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI64:
                        printk("EFI 64-bit system table pointer: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_SMBIOS:
                        printk("SMBIOS: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_ACPI_OLD:
                        acpi_v1 = kalloc(NULL, sizeof(struct acpi_descriptor_v1), 1);
                        if (acpi_v1 == NULL)
                                panic("OOM while allocating acpi_v1");
                        memcpy(acpi_v1, ((struct multiboot_tag_old_acpi *) tag)->rsdp,
                                        sizeof(struct acpi_descriptor_v1));
                        acpi_v1->oem_id[5] = '\0';

                        printk("ACPI v1 RSDP: rev=%d rsdt_addr=0x%x oem=%s\n",
                            acpi_v1->revision,
                            acpi_v1->rsdt_addr,
                            acpi_v1->oem_id);

                        break;

                        case MULTIBOOT_TAG_TYPE_ACPI_NEW:
                        acpi_v2 = kalloc(NULL, sizeof(struct acpi_descriptor_v2), 1);
                        if (acpi_v2 == NULL)
                                panic("OOM while allocating acpi_v2");
                        memcpy(acpi_v2, ((struct multiboot_tag_new_acpi *) tag)->rsdp,
                                        sizeof(struct acpi_descriptor_v2));
                        acpi_v2->oem_id[5] = '\0';

                        printk("ACPI v2 RSDP: rev=%d rsdt_addr=0x%x xsdt_addr=0x%X oem=%s\n",
                            acpi_v2->revision,
                            acpi_v2->rsdt_addr,
                            acpi_v2->xsdt_addr,
                            acpi_v2->oem_id);

                        break;

                        case MULTIBOOT_TAG_TYPE_NETWORK:
                        printk("Network: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI_MMAP:
                        printk("EFI memory map: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI_BS:
                        printk("EFI boot services not terminated: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI32_IH:
                        printk("EFI 32-bit image handle pointer: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_EFI64_IH:
                        printk("EFI 64-bit image handle pointer: size 0x%x\n", tag->size);
                        break;

                        case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
                        printk("Image load base address: 0x%x\n",
                                ((struct multiboot_tag_load_base_addr *) tag)->load_base_addr);
                        break;
                }
        }
}
