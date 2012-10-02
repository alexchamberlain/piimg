#include <stdio.h>
#include <parted/parted.h>

#include "builtin.h"
#include "loopdev.h"
#include "command.h"

static int cmd_find_unused(int argc, char* argv[]);
static int cmd_setup_device(int argc, char* argv[]);

static struct cmd_struct commands[] = {
  {"find_unused", cmd_find_unused},
  {"setup_device", cmd_setup_device}
};

static void print_usage() {
  const char usage_string[] =
    "piimg loopdev <find_unused, setup_device> [<img-file> <offset>]";

  printf("Usage:\n\t%s\n", usage_string);
}

static int cmd_find_unused(int argc, char* argv[]) {
  char * free_loop = loopdev_find_unused();

  printf("%s\n", free_loop);

  free(free_loop);

  return 0;
}

static int cmd_setup_device(int argc, char* argv[]) {
  char * free_loop = loopdev_find_unused();

  printf("Loop device: %s\n", free_loop);

  if(loopdev_setup_device(argv[0], atoi(argv[1]), free_loop)) {
    fprintf(stderr, "Failed to associate loop device (%s) to file (%s).\n", free_loop, argv[0]);
    goto error;
  }

  free(free_loop);

  return 0;

  error:
    free(free_loop);
    return 1;
}

int cmd_loopdev(int argc, char* argv[]) {
  if(argc == 0 || run_argv(commands, ARRAY_SIZE(commands), argc, argv)) {
    // No command supplied or invalid command, print usage.
    print_usage();
    return 1;
  }

  return 0;
}

