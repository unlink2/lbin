#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lbin.h"
#include "argtable2.h"

struct arg_lit *verb = NULL;
struct arg_lit *help = NULL;
struct arg_lit *version = NULL;

// arg end stores errors
struct arg_end *end = NULL;

#define lbin_argtable                                                          \
  { help, version, verb, end, }

void lbin_args_free(void) {
  void *argtable[] = lbin_argtable;
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void lbin_args_parse(int argc, char **argv) {
  help = arg_litn(NULL, "help", 0, 1, "display this help and exit");
  version = arg_litn(NULL, "version", 0, 1, "display version info and exit");
  verb = arg_litn("v", "verbose", 0, 1, "verbose output");
  end = arg_end(20);

  void *argtable[] = lbin_argtable;

  // output params
  char progname[] = "lbin";
  char short_desc[] = "";

  // version info
  int version_major = 0;
  int version_minor = 0;
  int version_patch = 1;

  int nerrors = arg_parse(argc, argv, argtable);
  int exitcode = 0;

  if (help->count > 0) {
    printf("Usage: %s", progname);
    arg_print_syntax(stdout, argtable, "\n");
    printf("%s\n\n", short_desc);
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    goto exit;
  }

  if (version->count) {
    printf("%s version %d.%d.%d\n", progname, version_major, version_minor,
           version_patch);
    goto exit;
  }

  if (nerrors > 0) {
    arg_print_errors(stdout, end, progname);
    printf("Try '%s --help' for more information.\n", progname);
    exitcode = 1;
    goto exit;
  }

  return;
exit:
  lbin_args_free();
  exit(exitcode); // NOLINT
}

int main(int argc, char **argv) {
  lbin_srand();
  lbin_args_parse(argc, argv);

  // map args to cfg here
  struct lbin_config cfg = lbin_config_defaults();

  cfg.verbose = verb->count > 0;

  int res = lbin_main(&cfg);

  lbin_args_free();
  return res;
}
