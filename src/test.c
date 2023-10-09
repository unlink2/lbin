#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include "lbin.h"

void test_lbin_join(void **state) {
  const char *res = lbin_join(strdup("prefix"), "/", "suffix");
  assert_string_equal("prefix/suffix", res);
  free((void *)res);
}

int main(int arc, char **argv) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_lbin_join),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
