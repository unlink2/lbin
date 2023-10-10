#include "lbin.h"
#include "env.h"
#include "platform.h"
#include <stdlib.h>
#include <string.h>

struct lbin_config lbin_config_defaults(void) {
  struct lbin_config cfg;
  memset(&cfg, 0, sizeof(cfg));

  char tmpnam_buf[TMP_MAX];

  cfg.base_path_len = PATH_MAX;
  cfg.file_path_len = PATH_MAX;

  cfg.put_headers = !getenv(LBIN_ENV_NO_HEADERS);
  cfg.echo = getenv(LBIN_ENV_ECHO) == NULL;
  cfg.in = lbin_fopen(lbin_getenv_or(LBIN_ENV_IN, LBIN_STDFILE), "re", stdin);
  cfg.out =
      lbin_fopen(lbin_getenv_or(LBIN_ENV_OUT, LBIN_STDFILE), "we", stdout);

  strncpy(cfg.base_path, lbin_getenv_or(LBIN_ENV_BASE_PATH, ""),
          cfg.base_path_len);

  strncpy(cfg.file_path,
          lbin_getenv_or(LBIN_ENV_FILE_PATH, lbin_tmpnam(tmpnam_buf, TMP_MAX)),
          cfg.file_path_len);

  return cfg;
}

struct lbin_ctx lbin_ctx_init(void) {
  struct lbin_ctx ctx;
  memset(&ctx, 0, sizeof(ctx));
  return ctx;
}

const char *lbin_tmpnam(char *dst, size_t len) {
  tmpnam(dst);
  return dst;
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
  }
}

void lbin_headers(FILE *f, struct lbin_ctx *ctx) {
  lbin_status_header(f, ctx->status);
  fprintf(f, "\n");
}

int lbin_main(struct lbin_config *cfg) {
  if (lbin_pledge() == -1) {
    fprintf(stderr, "Pledge failed!\n");
    return -1;
  }

  struct lbin_ctx ctx = lbin_ctx_init();

  if (cfg->put_headers) {
    lbin_headers(cfg->out, &ctx);
  }

  return 0;
}
