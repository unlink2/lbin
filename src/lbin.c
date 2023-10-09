#include "lbin.h"
#include "platform.h"
#include <string.h>

struct lbin_config lbin_config_defaults(void) {
  struct lbin_config cfg;
  memset(&cfg, 0, sizeof(cfg));

  cfg.put_headers = false;
  cfg.echo = false;
  cfg.in = stdin;
  cfg.out = stdout;

  return cfg;
}

char *lbin_join(char *dst, const char *path_sep, const char *suffix) {
  if (!dst || !suffix || !path_sep) {
    return NULL;
  }

  size_t len = strlen(dst) + strlen(suffix) + strlen(path_sep) + 1;
  char *new_dst = realloc(dst, len);
  strcat(new_dst, path_sep);
  strcat(new_dst, suffix);
  return new_dst;
}

int lbin_main(struct lbin_config *cfg) {
  if (lbin_pledge() == 0) {
    fprintf(stderr, "Pledge failed!\n");
    return -1;
  }
  return 0;
}
