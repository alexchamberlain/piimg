#include <stdio.h>
#include <parted/parted.h>

#include "builtin.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

struct cmd_struct {
  const char *cmd;
  int (*fn)(int, char **);
};

static struct cmd_struct commands[] = {
  {"list", cmd_list},
};

static void print_usage() {
  const char usage_string[] =
    "piimg <command> [<args]";

  printf("Usage:\n\t%s\n", usage_string);
}

int run_argv(int argc, char ** argv) {
  const char *cmd = argv[0];
  int i;

  ++argv;
  --argc;
  
  for(i = 0; i < ARRAY_SIZE(commands); ++i) {
    struct cmd_struct *p = commands+i;
    if(strcmp(p->cmd, cmd))
      continue;

    return p->fn(argc, argv);
  }

  // No command found...
  print_usage();
  return 1;
}

int main(int argc, char * argv[]) {
  ++argv;
  --argc;

  if(argc == 0) {
    // No command supplied, print usage.
    print_usage();
    return 1;
  } else {
    return run_argv(argc, argv);
  }
}
