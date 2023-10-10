#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include "lbin.h"

void test_lbin_join(void **state) {
  const size_t len = 256;
  char dst[len];
  memset(dst, 0, len);
  strncat(dst, "prefix", len);

  const char *res = lbin_join(dst, "/", "suffix", len);
  assert_string_equal("prefix/suffix", res);
}

void test_valid_filename(void **state) {
  size_t valid_len = strlen(LBIN_VALID_CHARS);
  assert_true(lbin_check_filename("test123", 7, LBIN_VALID_CHARS, valid_len));

  assert_false(lbin_check_filename("..st123", 7, LBIN_VALID_CHARS, valid_len));
  assert_false(lbin_check_filename("t..t123", 7, LBIN_VALID_CHARS, valid_len));
  assert_false(lbin_check_filename("test12.", 7, LBIN_VALID_CHARS, valid_len));
}

int main(int arc, char **argv) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_lbin_join),
      cmocka_unit_test(test_valid_filename),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
