#include <stdio.h>
#include <parted/parted.h>

#include "builtin.h"
#include "command.h"

static struct cmd_struct commands[] = {
  {"list",    cmd_list},
  {"loopdev", cmd_loopdev},
  {"mount",   cmd_mount},
};

static void print_usage() {
  const char usage_string[] =
    "piimg <command> [<args]";

  printf("Usage:\n\t%s\n", usage_string);
}

int main(int argc, char * argv[]) {
  ++argv;
  --argc;

  if(argc == 0 || run_argv(commands, ARRAY_SIZE(commands), argc, argv)) {
    // No command supplied or invalid command, print usage.
    print_usage();
    return 1;
  }

  return 0;
}
