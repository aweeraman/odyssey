/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cmd/shell.h>
#include <lib/stdio.h>
#include <lib/string.h>

#if ARCH_X86
#include <ppm/splash.h>
#include <sys/tty.h>
#include <x86/io.h>
#include <x86/boot/modules.h>
#endif

static char line[MAX_CMD_LENGTH];

static const struct kcmd valid_cmds[] = {
#if ARCH_X86
	{ .cmd="clear",     .func=cmd_clear },
	{ .cmd="splash",    .func=cmd_splash },
	{ .cmd="exception", .func=cmd_trigger_exception },
	{ .cmd="modules",   .func=cmd_modules },
	{ .cmd="canary",    .func=cmd_canary },
#endif
	{ .cmd="exit",      .func=cmd_exit },
	{ .cmd="help",      .func=cmd_help },
	{ .cmd="osver",     .func=cmd_osver },
};

#if ARCH_X86
int cmd_modules()
{
	print_boot_modules();
	return 0;
}

int cmd_canary()
{
	size_t i = 0;

	void (*canary)() = (void *) get_module_by_idx(0);

	if (canary == 0) {
		printk("Module not found!\n");
		return 1;
	}

	(*canary)();
	asm("\t movl %%eax, %0" : "=r"(i));

	if (i == CANARY_MAGIC_STRING)
		printk("The canary is alive.\n");
	else
		printk("The canary is dead.\n");

	return 0;
}

int cmd_clear()
{
	clear_screen();
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

#ifdef CONFIG_FRAMEBUFFER_RGB

	clear_screen();

	struct fb_info *fbi = get_fb_info();
	size_t pad_cols = 0;
	size_t pad_rows = 0;

	if ((fbi->width - PPM_COLS) > 0) {
		if ((fbi->height - PPM_ROWS) > 0) {
			pad_cols = (fbi->width  / 2) - (PPM_COLS / 2);
			pad_rows = (fbi->height / 2) - (PPM_ROWS / 2);
		}
	}

	for (int prows = 0; prows < PPM_ROWS; prows++) {
		for (int pcols = 0; pcols < PPM_COLS; pcols++) {

			size_t pos = ((prows * PPM_COLS) + pcols) * 3;
			size_t ci  = ((ppm_array[pos]   << 16) & 0xff0000) |
				     ((ppm_array[pos+1] << 8)  & 0xff00)   |
				      (ppm_array[pos+2]        & 0xff);

			draw_pixel(pad_rows + prows, pad_cols + pcols, ci);
		}
	}

	getchar(); /* block on input */

	clear_screen();

#endif /* CONFIG_FRAMEBUFFER_RGB */

	return 0;
}
#endif /* ARCH_X86 */

int cmd_exit()
{
	// TODO Use ACPI for shutdown on real hardware

#if ARCH_X86
	// in QEMU
	outw(0x604, 0x2000);

	// in Virtualbox
	outw(0x4004, 0x3400);
#endif

	// Won't get here, hopefully
	return 0;
}

int cmd_help()
{
	printk("Available commands:\n");
	for (size_t i = 0; i < (sizeof(valid_cmds) / sizeof(struct kcmd)); i++) {
		printk("  %s\n", valid_cmds[i].cmd);
	}
	return 0;
}

int cmd_osver()
{
	printk("Odyssey v%s\n",
		STRINGIFY(CONFIG_VERSION_MAJOR) "." \
		STRINGIFY(CONFIG_VERSION_MINOR));
	return 0;
}

static int run(const char *cmdline)
{
	for (size_t i = 0; i < (sizeof(valid_cmds) / sizeof(struct kcmd)); i++) {
		if (strncmp(valid_cmds[i].cmd, line, MAX_CMD_LENGTH) == 0)
			return (*valid_cmds[i].func)();
		else if (strncmp("", line, MAX_CMD_LENGTH) == 0)
			return 0;
	}

	printk ("Unknown command: %s\n", cmdline);
	return 1;
}

void start_interactive_shell()
{
	cmd_osver();

#if (CONFIG_KEYBOARD || CONFIG_SERIAL) && ARCH_X86
	while(1) {
		printk("# ");
		getstr(line, MAX_CMD_LENGTH);
		run(line);
	}
#elif ARCH_ARM
	printk("Serial input for ARM not supported yet\n");
#else
	printk("Enable keyboard driver for interactive shell\n");

	while(1) {
#if ARCH_X86
		asm("hlt");
#endif /* ARCH_X86 */
	}
#endif /* (CONFIG_KEYBOARD || CONFIG_SERIAL) && ARCH_ARM */
}
