#ifndef lbinH__
#define lbinH__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "env.h"

#define LBIN_STDFILE "-"
#define LBIN_TMP_MAX 16
#define LBIN_PATH_MAX PATH_MAX

#define LBIN_KEY_MAX 2048

extern const char *LBIN_VALID_CHARS;

struct lbin_config {
  int ok;
  bool verbose;

  const char *valid_filename_chars;
  size_t valid_filename_chars_len;

  // set to true to echo output back to stdout
  // if out is not stdout
  bool echo;
  bool put_headers;
  bool check_file_name;

  const char *mime_type;

  // the requried key
  const char *key;

  // user provided key
  const char *usr_key;

  // TODO: allow multiple key files
  // username=key
  // a user may have multiple keys.
  // if no keyfile is provided do not use any authentication
  // that adds username to the base path if present
  const char *key_file;
  const char *base_path;
  const char *in_path;
  // internal buffers for paths
  char out_path[LBIN_PATH_MAX];
};

enum lbin_status { LBIN_OK, LBIN_BAD_REQUEST, LBIN_NOT_FOUND, LBIN_FORBIDDEN };

struct lbin_ctx {
  enum lbin_status status;

  // input file
  FILE *in;

  // out is the target file
  FILE *out;
};

// sets up config from env variables
// see env.h for all possible variables
struct lbin_config lbin_config_from_env(void);

struct lbin_ctx lbin_ctx_init(struct lbin_config *cfg);

// verify a filename only contains valid chars
bool lbin_check_filename(const char *filename, size_t len,
                         const char *valid_chars, size_t valid_chars_len);

// open a file at base+path or if path matches LBIN_STDFILE return FILE *or
FILE *lbin_fopen(const char *base, const char *path, const char *mode,
                 FILE * or);
const char *lbin_getenv_or(const char *env, const char * or);

// creates a random string
// of the following pattern:
// a-z0-9*
const char *lbin_tmpnam(char *dst, size_t len, const char *valid_chars,
                        const size_t valid_chars_len);

char *lbin_join(char *dst, char path_sep, const char *suffix, size_t len);

int lbin_rand(void);

bool lbin_auth(const char *key, const char *usr_key);
// pipes all data from dst to src file
// optioanlly can echo to stdout if dst is not stdout
int lbin_pipe(FILE *dst, FILE *src, bool echo);

int lbin_main(struct lbin_config *cfg);

#endif
