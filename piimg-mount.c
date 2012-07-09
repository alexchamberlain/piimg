#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mount.h>

#include "builtin.h"
#include "command.h"
#include "partition.h"

static void print_usage() {
  const char usage_string[] =
    "piimg mount <img-file> <mount-point>";

  printf("Usage:\n\t%s\n", usage_string);
}

int cmd_mount(int argc, char* argv[]) {
  char *boot_loop;
  char *root_loop;
  struct piimg_img simg;
  char *boot_mount_point = NULL;
  char *root_mount_point = argv[1];
  int strlen_root_mount_point = strlen(root_mount_point);
  memset(&simg, 0, sizeof(struct piimg_img));


  if(analyse_img(&simg, argv[0])) {
    fprintf(stderr, "Failed to analyse image.\n");
    goto error;
  }

  if((boot_loop = loopdev_find_unused()) == NULL) {
    fprintf(stderr, "Failed to find a free loop device for the boot partition.\n");
    goto error;
  }

  if(loopdev_setup_device(argv[0], simg.boot_offset, boot_loop)) {
    fprintf(stderr, "Failed to associate loop device (%s) to file (%s).\n", boot_loop, argv[0], simg.boot_offset);
    goto error;
  }

  if((root_loop = loopdev_find_unused()) == NULL) {
    fprintf(stderr, "Failed to find a free loop device for the root partition.\n");
    goto error;
  }

  printf("Root Loop device: %s\n", root_loop);
  printf("Boot Loop device: %s\n", boot_loop);

  if(loopdev_setup_device(argv[0], simg.root_offset, root_loop)) {
    fprintf(stderr, "Failed to associate loop device (%s) to file (%s; offset=%lld).\n", root_loop, argv[0], simg.root_offset);
    goto error;
  }

  printf("Root mount point: %s\n", root_mount_point);

  if(mount(root_loop, root_mount_point, "ext4", 0, NULL) != 0) {
    fprintf(stderr, "Failed to mount loop device (%s) to mount point (%s).\n", root_loop, root_mount_point);
    goto error;
  }

  boot_mount_point = malloc(strlen_root_mount_point + strlen("/boot") + 1);
  if(boot_mount_point == NULL) {
    fprintf(stderr, "Failed to allocate memory for boot mount point.\n");
    goto error;
  }

  strncpy(boot_mount_point, root_mount_point, strlen_root_mount_point);
  strncpy(boot_mount_point + strlen_root_mount_point, "/boot", strlen("/boot"));
  *(boot_mount_point + strlen_root_mount_point + strlen("/boot")) = '\0';

  printf("Boot mount point: %s\n", boot_mount_point);

  if(mount(boot_loop, boot_mount_point, "vfat", 0, "") != 0) {
    fprintf(stderr, "Failed to mount loop device (%s) to mount point (%s).\n", boot_loop, boot_mount_point);
    fprintf(stderr, "Error (%d) %s\n", errno, strerror(errno));
    goto error;
  }

  free(boot_loop);
  free(root_loop);
  free(boot_mount_point);

  return 0;

  error:
    free(boot_loop);
    free(root_loop);
    free(boot_mount_point);

    /* TODO: Should clean up allocated loop devices too. */

    return 1;
}

