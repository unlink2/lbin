#include "lbin.h"
#include "env.h"
#include <stdlib.h>
#include <string.h>

const char *LBIN_VALID_CHARS = "0123456789abcdefghijklmnopqrstuvwxyz";

struct lbin_config lbin_config_defaults(void) {
  size_t valid_chars_len = strlen(LBIN_VALID_CHARS);

  struct lbin_config cfg;
  memset(&cfg, 0, sizeof(cfg));

  char tmpnam_buf[LBIN_TMP_MAX];

  cfg.valid_filename_chars = LBIN_VALID_CHARS;
  cfg.valid_filename_chars_len = valid_chars_len;

  cfg.put_headers = !getenv(LBIN_ENV_NO_HEADERS);
  cfg.check_file_name = !getenv(LBIN_ENV_NO_CHK_FILENAME);

  cfg.echo = getenv(LBIN_ENV_ECHO) == NULL;

  strncpy(cfg.out_path, lbin_getenv_or(LBIN_ENV_OUT, LBIN_STDFILE),
          LBIN_PATH_MAX);

  char base_path[LBIN_PATH_MAX];
  strncpy(base_path, lbin_getenv_or(LBIN_ENV_BASE_PATH, ""), LBIN_PATH_MAX);

  char file_path[LBIN_PATH_MAX];
  strncpy(file_path,
          lbin_getenv_or(LBIN_ENV_FILE_PATH,
                         lbin_tmpnam(tmpnam_buf, LBIN_TMP_MAX,
                                     cfg.valid_filename_chars,
                                     cfg.valid_filename_chars_len)),
          LBIN_PATH_MAX);

  // verify input data in cfg

  if (cfg.check_file_name &&
      !lbin_check_filename(file_path, LBIN_PATH_MAX, cfg.valid_filename_chars,
                           cfg.valid_filename_chars_len)) {
    cfg.ok = -1;
  }

  return cfg;
}

struct lbin_ctx lbin_ctx_init(void) {
  struct lbin_ctx ctx;
  memset(&ctx, 0, sizeof(ctx));
  return ctx;
}

// FIXME: do we need a better rand function?
int lbin_rand(void) {
  return rand(); // NOLINT
}

const char *lbin_tmpnam(char *dst, size_t len, const char *valid_chars,
                        const size_t valid_chars_len) {

  memset(dst, 0, len);
  for (size_t i = 0; i < len; i++) {

    dst[i] = valid_chars[(lbin_rand() % (valid_chars_len - 1))];
  }

  dst[len - 1] = '\0';

  return dst;
}

bool lbin_check_filename(const char *filename, size_t len,
                         const char *valid_chars, size_t valid_chars_len) {
  for (size_t fi = 0; fi < len && filename[fi]; fi++) {
    bool any = false;
    for (size_t vi = 0; vi < valid_chars_len; vi++) {
      if (filename[fi] == valid_chars[vi]) {
        any = true;
        break;
      }
    }

    if (!any) {
      return false;
    }
  }
  return true;
}

char *lbin_join(char *dst, const char *path_sep, const char *suffix,
                size_t len) {
  if (!dst || !suffix || !path_sep) {
    return NULL;
  }

  strncat(dst, path_sep, len);
  strncat(dst, suffix, len);
  return dst;
}

FILE *lbin_fopen(const char *path, const char *mode, FILE * or) {
  if (strncmp(path, LBIN_STDFILE, strlen(path)) == 0) {
    return or ;
  }

  return fopen(path, mode);
}

const char *lbin_getenv_or(const char *env, const char * or) {
  const char *val = getenv(env);
  if (!val) {
    return or ;
  }
  return val;
}

void lbin_status_header(FILE *f, enum lbin_status s) {
  fprintf(f, "Status: ");
  switch (s) {
  case LBIN_OK:
    fputs("200 OK", f);
    break;
  case LBIN_NOT_FOUND:
    fputs("404 Not found", f);
    break;
  case LBIN_BAD_REQUEST:
    fputs("400 Bad Request", f);
    break;
  }
}

void lbin_headers(FILE *f, struct lbin_ctx *ctx) {
  lbin_status_header(f, ctx->status);
  fprintf(f, "\n");
}

int lbin_pipe(FILE *dst, FILE *src, bool echo) {
  const size_t buf_len = 4096;
  char buf[buf_len];
  memset(buf, 0, buf_len);

  size_t read = 0;
  while ((read = fread(buf, 1, buf_len, src))) {
    fwrite(buf, 1, read, dst);

    if (echo && dst != stdout) {
      fwrite(buf, 1, read, stdout);
    }
  }

  return 0;
}

int lbin_main(struct lbin_config *cfg) {
  if (lbin_srand() == -1) {
    return -1;
  }
  if (lbin_pledge() == -1) {
    fprintf(stderr, "Pledge failed!\n");
    return -1;
  }

  struct lbin_ctx ctx = lbin_ctx_init();

  if (cfg->ok == -1) {
    ctx.status = LBIN_BAD_REQUEST;
  }

  if (cfg->verbose) {
    fprintf(stderr, "int: %s out: %s\n", cfg->in_path, cfg->out_path);
  }

  FILE *out = lbin_fopen(cfg->out_path, "we", stdout);

  if (cfg->put_headers) {
    lbin_headers(out, &ctx);
  }

  FILE *in = lbin_fopen(cfg->in_path, "re", stdin);

  if (in && in != stdin) {
    fclose(in);
  }

  if (out && out != stdout) {
    fclose(out);
  }

  return ctx.status;
}
