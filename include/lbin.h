#ifndef lbinH__
#define lbinH__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "env.h"

#define LBIN_STDFILE "-"
#define LBIN_TMP_MAX 16
#define LBIN_PATH_MAX PATH_MAX

extern const char *LBIN_VALID_CHARS;

struct lbin_config {
  bool verbose;

  FILE *in;
  FILE *out;

  size_t base_path_len;
  char base_path[LBIN_PATH_MAX];

  size_t file_path_len;
  char file_path[LBIN_PATH_MAX];

  const char *valid_filename_chars;
  size_t valid_filename_chars_len;

  // set to true to echo output back to stdout
  // if out is not stdout
  bool echo;
  bool put_headers;
  bool check_file_name;
};

enum lbin_status {
  LBIN_OK,
  LBIN_BAD_REQUEST,
  LBIN_NOT_FOUND,
};

struct lbin_ctx {
  enum lbin_status status;
};

struct lbin_config lbin_config_defaults(void);

struct lbin_ctx lbin_ctx_init(void);

// verify a filename only contains valid chars
bool lbin_check_filename(const char *filename, size_t len,
                         const char *valid_chars, size_t valid_chars_len);

FILE *lbin_fopen(const char *path, const char *mode, FILE * or);
const char *lbin_getenv_or(const char *env, const char * or);

// creates a random string
// of the following pattern:
// a-z0-9*
const char *lbin_tmpnam(char *dst, size_t len, const char *valid_chars,
                        const size_t valid_chars_len);

char *lbin_join(char *dst, const char *path_sep, const char *suffix,
                size_t len);

int lbin_rand(void);

int lbin_main(struct lbin_config *cfg);

#endif
