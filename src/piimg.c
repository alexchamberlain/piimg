#include <stdio.h>
#include <parted/parted.h>
#include <unistd.h>
#include <sys/types.h>

#include <piimg.h>
#include "builtin.h"

static struct cmd_struct commands[] = {
  {"list",    cmd_list},
  {"loopdev", cmd_loopdev},
  {"mount",   cmd_mount},
  {"umount",  cmd_umount},
  {"grow",    cmd_grow},
  {"dd",      cmd_dd},
};

static void print_usage() {
  const char usage_string[] =
    "piimg <command> [<args]";

  printf("Usage:\n\t%s\n", usage_string);
}

int main(int argc, char * argv[]) {

  // Check that we are running as root.
  if(geteuid() != 0) {
    fprintf(stderr, "piimg should be run with the effective user ID of 0.\n");
    return 1;
  }

  // Drop priveleges until needed.
  if(seteuid(getuid()) == -1 || geteuid() != getuid()) {
    fprintf(stderr, "Failed to drop priveleges.\n");
    return 1;
  }

  ++argv;
  --argc;

  if(argc == 0 || run_argv(commands, ARRAY_SIZE(commands), argc, argv)) {
    // No command supplied or invalid command, print usage.
    print_usage();
    return 1;
  }

  return 0;
}
