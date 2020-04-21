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
#include "libk.h"
#include "memory.h"

static int tests_passed;
static int tests_failed;

static void fail(const char *test, const char *str) {
  printf("TEST FAIL %s: %s\n", test, str);
}

static int memcpy_1() {
  int ret = 0;
  char str1[5];
  char *str2 = "123";
  char *str3 = "12345";
  char str4[5] = { '1', '2', '\0', '3', '4' };
  char str5[5] = { 'p', 'p', 'p', 'p', 'p' };
  void *p;

  p = memcpy(str1, str2, 5);
  if (strnlen(str1, 5) != 3) {
    fail(__FUNCTION__, "string length doesn't match, expected 3");
    ret = 1;
  }
  if (p != str1) {
    fail(__FUNCTION__, "invalid pointer returned");
    ret = 2;
  }
  memcpy(str1, str3, 5);
  if (!(str1[0] == '1' && str1[1] == '2' && str1[2] == '3' && str1[3] == '4' && str1[4] == '5')) {
    fail(__FUNCTION__, "string is not identical");
    ret = 3;
  }
  memcpy(str5, str4, 5);
  if (!(str5[0] == '1' && str5[1] == '2' && str5[2] == '\0' && str5[3] == '3' && str5[4] == '4')) {
    fail(__FUNCTION__, "string is not identical, interleaving null");
    ret = 4;
  }
  return ret;
}

static int memset_1() {
  int ret = 0;
  char str[3];
  void *p;

  p = memset(str, 'a', sizeof(str));
  if (!(str[0] == 'a' && str[1] == 'a' && str[2] == 'a')) {
    fail(__FUNCTION__, "all elements are not updated correctly");
    ret = 1;
  }
  if (p != str) {
    fail(__FUNCTION__, "invalid pointer returned");
    ret = 2;
  }

  return ret;
}

static int strnlen_1() {
  int ret = 0;
  char *str1 = "123";
  char *str2 = "12345";
  char *str3 = "123456";
  if (strnlen(str1, 5) != 3) {
    fail(__FUNCTION__, "string length doesn't match, expected 3");
    ret = 1;
  }
  if (strnlen(str2, 5) != 5) {
    fail(__FUNCTION__, "string length doesn't match, expected 5");
    ret = 2;
  }
  if (strnlen(str3, 5) != 5) {
    fail(__FUNCTION__, "string length doesn't match, expected 5, not 6");
    ret = 3;
  }
  return ret;
}

static int strncpy_1() {
  int ret = 0;
  char str1[5];
  char *str2 = "123";
  char *str3 = "12345";
  char str4[5] = { '1', '2', '\0', '3' };
  char str5[5] = { 'p', 'p', 'p', 'p', 'p' };
  void *p;

  p = strncpy(str1, str2, 5);
  if (strnlen(str1, 5) != 3) {
    fail(__FUNCTION__, "string length doesn't match, expected 3");
    ret = 1;
  }
  if (p != str1) {
    fail(__FUNCTION__, "invalid pointer returned");
    ret = 2;
  }
  strncpy(str1, str2, 5);
  if (!(str1[0] == '1' && str1[1] == '2' && str1[2] == '3' && str1[3] == '\0')) {
    fail(__FUNCTION__, "string is not identical/null terminated");
    ret = 3;
  }
  strncpy(str1, str3, 5);
  if (!(str1[0] == '1' && str1[1] == '2' && str1[2] == '3' && str1[3] == '4' && str1[4] == '5')) {
    fail(__FUNCTION__, "string is not identical");
    ret = 4;
  }
  strncpy(str5, str4, 5);
  if (!(str5[0] == '1' && str5[1] == '2' && str5[2] == '\0' && str5[3] == '\0' && str5[4] == '\0')) {
    fail(__FUNCTION__, "copy doesn't honor null");
    ret = 5;
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
    ret = 2;
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
  run(strncpy_1);
  run(memset_1);
  run(memcpy_1);
#ifdef CONFIG_ARCH_X86_32
  run(multiboot2_magic_1);
#endif

  printf("TESTS %d / %d passed\n", tests_passed, tests_passed+tests_failed);
}

