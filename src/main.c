#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lbin.h"
#include "argtable2.h"

struct arg_lit *verb = NULL;
struct arg_lit *help = NULL;
struct arg_lit *version = NULL;

struct arg_lit *no_echo = NULL;
struct arg_lit *no_headers = NULL;
struct arg_lit *no_filename_chk = NULL;

struct arg_file *in = NULL;
struct arg_file *out = NULL;
struct arg_file *base_path = NULL;

// arg end stores errors
struct arg_end *end = NULL;

#define lbin_argtable                                                          \
  {                                                                            \
    help, version, verb, no_echo, no_headers, no_filename_chk, in, out,        \
        base_path, end,                                                        \
  }

void lbin_args_free(void) {
  void *argtable[] = lbin_argtable;
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void lbin_args_parse(int argc, char **argv) {
  help = arg_litn(NULL, "help", 0, 1, "display this help and exit");
  version = arg_litn(NULL, "version", 0, 1, "display version info and exit");
  verb = arg_litn("v", "verbose", 0, 1, "verbose output");

  no_echo = arg_lit0(NULL, "no-echo", "disable echo");
  no_headers = arg_lit0(NULL, "no-headers", "disable http headers on stdout");
  no_filename_chk = arg_lit0(
      NULL, "no-filenamechk",
      "disable filename restrictions. Warning: if used as a CGI script this "
      "may allow users to traverse the entire file system!");

  in = arg_file0("i", "in", "FILE", "input file");
  out = arg_file0("o", "out", "FILE", "output file");
  base_path = arg_file0("b", "base", "DIR", "base directory");

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
  if (lbin_srand() == -1) {
    return -1;
  }
  lbin_args_parse(argc, argv);

  // map args to cfg here
  struct lbin_config cfg = lbin_config_defaults();

  if (verb->count > 0) {
    cfg.verbose = verb->count > 0;
  }

  if (no_echo->count > 0) {
    cfg.echo = false;
  }

  if (no_filename_chk->count > 0) {
    cfg.check_file_name = false;
  }

  if (no_headers->count > 0) {
    cfg.put_headers = false;
  }

  if (in->count > 0) {
    strncpy(cfg.in_path, in->filename[0], LBIN_PATH_MAX);
  }

  if (out->count > 0) {
    strncpy(cfg.out_path, out->filename[0], LBIN_PATH_MAX);
  }

  if (base_path->count > 0) {
    strncpy(cfg.base_path, base_path->filename[0], LBIN_PATH_MAX);
  }

  int res = lbin_main(&cfg);

  lbin_args_free();
  return res;
}
