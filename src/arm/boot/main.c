/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <lib/string.h>
#include <lib/stdio.h>

#ifdef CONFIG_TEST
#include <test.h>
#endif

void kernel_main() {
        printf("\nOdyssey v%s\n",
                        STRINGIFY(CONFIG_VERSION_MAJOR) "." \
                        STRINGIFY(CONFIG_VERSION_MINOR));

#ifdef CONFIG_TEST
        run_tests();
#endif
}
