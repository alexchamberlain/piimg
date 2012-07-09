#include <stdio.h>
#include <parted/parted.h>

#include <sys/mount.h>

#include "builtin.h"
#include "command.h"

static void print_usage() {
  const char usage_string[] =
    "piimg mount <img-file> <offset> <mount-point>";

  printf("Usage:\n\t%s\n", usage_string);
}

int cmd_mount(int argc, char* argv[]) {
  char * free_loop = loopdev_find_unused();

  printf("Loop device: %s\n", free_loop);

  if(loopdev_setup_device(argv[0], atoi(argv[1]), free_loop)) {
    fprintf(stderr, "Failed to associate loop device (%s) to file (%s).\n", free_loop, argv[0]);
    goto error;
  }

  if(mount(free_loop, argv[2], "ext4", 0, NULL) != 0) {
    fprintf(stderr, "Failed to mount loop device (%s) to mount point (%s).\n", free_loop, argv[2]);
    goto error;
  }

  free(free_loop);

  return 0;

  error:
    free(free_loop);
    return 1;
}

