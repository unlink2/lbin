#ifndef lbinH__
#define lbinH__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct lbin_config {
  bool verbose;

  FILE *in;
  FILE *out;

  // set to true to echo output back to stdout
  // if out is not stdout
  bool echo;
  bool put_headers;
};

struct lbin_config lbin_config_defaults(void);

char *lbin_join(char *dst, const char *path_sep, const char *suffix,
                size_t len);

int lbin_main(struct lbin_config *cfg);

#endif
