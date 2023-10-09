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

enum lbin_status {
  LBIN_OK,
  LBIN_NOT_FOUND,
};

struct lbin_ctx {
  enum lbin_status status;
};

struct lbin_config lbin_config_defaults(void);

struct lbin_ctx lbin_ctx_init(void);

char *lbin_join(char *dst, const char *path_sep, const char *suffix,
                size_t len);

int lbin_main(struct lbin_config *cfg);

#endif
