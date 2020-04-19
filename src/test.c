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

#include "test.h"
#include "tty.h"
#include "string.h"
#include "memory.h"

static int tests_passed;
static int tests_failed;

static void fail(const char *test, const char *str) {
  printk("Test fail %s: %s\n", test, str);
}

static int strnlen_1() {
  int ret = 0;
  char *str = "123";
  if (strnlen(str, 5) != 3) {
    fail(__FUNCTION__, "string length doesn't match");
    ret = 1;
  }
  return ret;
}

static int multiboot2_magic_1() {
  int ret = 0;
  mem_ptr_t *p = (mem_ptr_t *) 0x100000;
  // Checking for the multiboot 2 magic string
  if (p->word.w1 != 0x50d6) {
    fail(__FUNCTION__, "peek at mem location 0x100000 is not 50d6");
    ret = 1;
  }
  if (p->word.w2 != 0xe852) {
    fail(__FUNCTION__, "peek at mem location 0x100002 is not e852");
    ret = 1;
  }
  return ret;
}

static void run(int (*test)()) {
  int ret = (*test)();
  if (ret == 0) {
    tests_passed++;
  } else {
    tests_failed++;
  }
}

void run_tests() {
  tests_passed = 0;
  tests_failed = 0;

  run(strnlen_1);
#ifdef CONFIG_ARCH_X86_32
  run(multiboot2_magic_1);
#endif

  printk("Tests %d / %d passed\n", tests_passed, tests_passed+tests_failed);
}

