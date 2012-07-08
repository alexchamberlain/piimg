#include <stdio.h>
#include <parted/parted.h>

#include "builtin.h"
#include "command.h"

static int cmd_find_unused(int argc, char* argv[]);

static struct cmd_struct commands[] = {
  {"find_unused", cmd_find_unused}
};

static void print_usage() {
  const char usage_string[] =
    "piimg loopdev find_unused";

  printf("Usage:\n\t%s\n", usage_string);
}

static int cmd_find_unused(int argc, char* argv[]) {
  char * free_loop = loopdev_find_unused();

  printf("%s\n", free_loop);

  free(free_loop);

  return 0;
}

int cmd_loopdev(int argc, char* argv[]) {
  if(argc == 0 || run_argv(commands, ARRAY_SIZE(commands), argc, argv)) {
    // No command supplied or invalid command, print usage.
    print_usage();
    return 1;
  }

  return 0;
}

