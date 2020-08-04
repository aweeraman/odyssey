/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <lib/stdio.h>
#include <lib/string.h>
#include <sys/tty.h>
#include <sys/keyboard.h>
#include <sys/serial.h>

uint8_t putchar(uint8_t c)
{
	printc(c);
	return c;
}

#if ARCH_X86
uint8_t getchar()
{
#if (! defined CONFIG_KEYBOARD) && (defined CONFIG_SERIAL)
	return read_serial();
#endif
	return block_and_read_char();
}

char *getstr(char *line, int max_length)
{
	uint8_t ch;
	int line_counter = 0;

	ch = getchar();
	while(ch != '\n' && ch != '\r') {
		if (line_counter < max_length-1) {
			if (ch == '\b') {
				if (line_counter > 0) {
					line[line_counter--] = '\0';
					putchar(ch);
				}
			} else {
				line[line_counter++] = ch;
				putchar(ch);
			}
		} else {
			putchar('\n');
			line[line_counter] = '\0';
			line_counter = 0;
		}
		ch = getchar();
	}
	line[line_counter] = '\0';
	line_counter = 0;
	putchar(ch);

	return line;
}
#endif /* ARCH_X86 */

/*
 * Like puts, without a newline at the end
 */
int prints(char *str)
{
	size_t i = 0;
	for (; str[i] != '\0'; i++) {
		printc(str[i]);
	}
	return i;
}

void printk(const char *fmt, ...)
{
	int	    i;
	size_t      ul;
	uint64_t    ull;
	char       *str;
	char	    print_buf[128];
	const char *p;

	va_list arg;
	va_start(arg, fmt);

	for(p = fmt; *p != '\0'; p++) {

		if(*p != '%') {
			putchar(*p);
			continue;
		}

		switch(*++p) {

		case 'c':
			i = va_arg(arg, int);
			putchar(i);
			break;

		case 's':
			str = va_arg(arg, char*);
			prints(str);
			break;

		case 'b':
			ul = va_arg(arg, size_t);
			prints(itoa((size_t) ul, print_buf, 2));
			break;

		case 'o':
			ul = va_arg(arg, size_t);
			prints(itoa((size_t) ul, print_buf, 8));
			break;

		case 'i':
		case 'd':
			i = va_arg(arg, int);
			if (i < 0) {
				i *= -1;
				putchar('-');
			}
			prints(itoa((int) i, print_buf, 10));
			break;

		case 'u':
			ul = va_arg(arg, size_t);
			prints(itoa((size_t) ul, print_buf, 10));
			break;

		case 'x':
			ul = va_arg(arg, size_t);
			prints(itoa((size_t) ul, print_buf, 16));
			break;

		case 'X':
			ull = va_arg(arg, uint64_t);
			prints(itoa((uint64_t) ull, print_buf, 16));
			break;

		case '%':
			prints("%");
			break;
		}
	}

	va_end(arg);
}
