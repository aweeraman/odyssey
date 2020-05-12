/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <odyssey.h>
#include <lib/termio.h>

#ifdef CONFIG_TEST
#include <test.h>
#endif

void kernel_main() {
        printf("\nOdyssey v%s\n", CONFIG_VERSION);

#ifdef CONFIG_TEST
        run_tests();
#endif
}
