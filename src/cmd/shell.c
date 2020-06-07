/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cmd/shell.h>
#include <lib/stdio.h>
#include <lib/string.h>
#include <ppm/odyssey.h>
#include <sys/tty.h>
#include <x86/io.h>
#include <x86/boot/modules.h>

static char line[MAX_CMD_LENGTH];

static const cmd_t valid_cmds[] = {
        { .cmd="clear",     .func=cmd_clear },
        { .cmd="splash",    .func=cmd_splash },
        { .cmd="exit",      .func=cmd_exit },
        { .cmd="exception", .func=cmd_trigger_exception },
        { .cmd="help",      .func=cmd_help },
        { .cmd="modules",   .func=cmd_modules },
        { .cmd="canary",    .func=cmd_canary },
};

int cmd_modules()
{
        print_boot_modules();
        return 0;
}

int cmd_canary()
{
        uint32_t i = 0;
        module_t canary = (module_t) get_module_by_idx(0);
        if (canary == 0) {
                printk("Module not found!\n");
                return 1;
        }
        canary();
        asm("\t movl %%eax, %0" : "=r"(i));
        printk("Canary said 0x%x\n", i);
        return 0;
}

int cmd_clear()
{
        clear_screen();
        return 0;
}

int cmd_exit()
{
        // TODO Use ACPI for shutdown on real hardware

        // in QEMU
        outw(0x604, 0x2000);

        // in Virtualbox
        outw(0x4004, 0x3400);

        // Won't get here, hopefully
        return 0;
}

int cmd_trigger_exception()
{
        // Trigger a division by zero
        printk("%d", 1/0);

        // Won't get here
        return 0;
}

int cmd_splash()
{
        clear_screen();
#ifdef CONFIG_FRAMEBUFFER_RGB
        for (int prows = 0; prows < PPM_ROWS; prows++) {
                for (int pcols = 0; pcols < PPM_COLS; pcols++) {
                        uint32_t pos = ((prows * pcols) + pcols) * 3;
                        char r = ppm_array[pos];
                        char g = ppm_array[pos+1];
                        char b = ppm_array[pos+2];
                        int ri = (r << 16) & 0xff0000;
                        int gi = (g << 8) & 0xff00;
                        int bi = b & 0xff;
                        uint32_t ci = 0x00000000 | ri | gi | bi;
                        draw_pixel(prows, pcols, ci);
                }
        }
#endif
        return 0;
}

int cmd_help()
{
        printk("Available commands:\n");
        for (size_t i = 0; i < (sizeof(valid_cmds) / sizeof(cmd_t)); i++) {
                printk("  %s\n", valid_cmds[i].cmd);
        }
        return 0;
}

static int run(const char *cmdline)
{
        for (size_t i = 0; i < (sizeof(valid_cmds) / sizeof(cmd_t)); i++) {
                if (strncmp(valid_cmds[i].cmd, line, MAX_CMD_LENGTH) == 0) {
                        return (*valid_cmds[i].func)();
                } else if (strncmp("", line, MAX_CMD_LENGTH) == 0) {
                        return 0;
                }
        }

        printk ("Unknown command: %s\n", cmdline);
        return 1;
}

void start_interactive_shell()
{
        printk("\nOdyssey v%s\n",
                        STRINGIFY(CONFIG_VERSION_MAJOR) "." \
                        STRINGIFY(CONFIG_VERSION_MINOR));

#if CONFIG_KEYBOARD || CONFIG_SERIAL
        while(1) {
                printk("# ");
                getstr(line, MAX_CMD_LENGTH);
                run(line);
        }
#else
        printk("Enable keyboard driver for interactive shell\n");

        while(1) {
#if ARCH_X86
                asm("hlt");
#endif
        }
#endif
}
