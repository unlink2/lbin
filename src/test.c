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

int main(int arc, char **argv) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_lbin_join),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
