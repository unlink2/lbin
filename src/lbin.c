#include "lbin.h"
#include "platform.h"
#include <string.h>

struct lbin_config lbin_config_defaults(void) {
  struct lbin_config cfg;
  memset(&cfg, 0, sizeof(cfg));

  cfg.put_headers = true;
  cfg.echo = false;
  cfg.in = stdin;
  cfg.out = stdout;

  return cfg;
}

struct lbin_ctx lbin_ctx_init(void) {
  struct lbin_ctx ctx;
  memset(&ctx, 0, sizeof(ctx));
  return ctx;
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
