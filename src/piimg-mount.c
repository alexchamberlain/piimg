#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mount.h>

#include <piimg.h>
#include "builtin.h"

static void print_usage() {
  const char usage_string[] =
    "piimg mount <img-file> <mount-point>";

  printf("Usage:\n\t%s\n", usage_string);
}

int cmd_mount(int argc, char* argv[]) {
  char *boot_loop = NULL;
  char *root_loop = NULL;
  struct piimg_img simg;

  FSTR_DECLARE_WRAPPER(mnt_root, argv[1]);
  FSTR_DECLARE_NULL   (mnt_boot);
  FSTR_DECLARE_NULL   (mnt_dev);
  FSTR_DECLARE_NULL   (mnt_proc);
  FSTR_DECLARE_NULL   (mnt_sys);

  const FSTR_DECLARE_WRAPPER(dir_boot, "/boot");
  const FSTR_DECLARE_WRAPPER(dir_dev,  "/dev");
  const FSTR_DECLARE_WRAPPER(dir_proc, "/proc");
  const FSTR_DECLARE_WRAPPER(dir_sys,  "/sys");

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
    fprintf(stderr, "Failed to associate loop device (%s) to file (%s) at offset (%ld).\n", boot_loop, argv[0], simg.boot_offset);
    goto error;
  }

  if((root_loop = loopdev_find_unused()) == NULL) {
    fprintf(stderr, "Failed to find a free loop device for the root partition.\n");
    goto error;
  }

  printf("Root Loop device: %s\n", root_loop);
  printf("Boot Loop device: %s\n", boot_loop);

  if(loopdev_setup_device(argv[0], simg.root_offset, root_loop)) {
    fprintf(stderr, "Failed to associate loop device (%s) to file (%s; offset=%ld).\n", root_loop, argv[0], simg.root_offset);
    goto error;
  }

  if(escalate()) goto error;

  if(mount(root_loop, mnt_root.c_str, "ext4", 0, NULL) != 0) {
    fprintf(stderr, "Failed to mount loop device (%s) to mount point (%s).\n", root_loop, mnt_root.c_str);
    goto error;
  }

  if(drop()) goto error;

  if(fstrcat(&mnt_boot, &mnt_root, &dir_boot)
    || fstrcat(&mnt_dev,  &mnt_root, &dir_dev)
    || fstrcat(&mnt_proc, &mnt_root, &dir_proc)
    || fstrcat(&mnt_sys,  &mnt_root, &dir_sys)) {
    fprintf(stderr, "Failed to construct mount points.\n");
    goto error;
  }

  printf("Mount Points\n");
  printf("============\n");
  printf("/    : %s\n", mnt_root.c_str);
  printf("/boot: %s\n", mnt_boot.c_str);
  printf("/dev : %s\n", mnt_dev.c_str);
  printf("/proc: %s\n", mnt_proc.c_str);
  printf("/sys : %s\n", mnt_sys.c_str);

  if(escalate()) goto error;

  if(mount(boot_loop, mnt_boot.c_str, "vfat", 0, NULL) != 0) {
    fprintf(stderr, "Failed to mount loop device (%s) to mount point (%s).\n", boot_loop, mnt_boot.c_str);
    fprintf(stderr, "Error (%d) %s\n", errno, strerror(errno));
    goto error;
  }

  if(mount("/dev", mnt_dev.c_str, NULL, MS_BIND|MS_REC, NULL) != 0) {
    fprintf(stderr, "Failed to mount loop device (%s) to mount point (%s).\n", "/dev", mnt_dev.c_str);
    fprintf(stderr, "Error (%d) %s\n", errno, strerror(errno));
    goto error;
  }

  if(mount("none", mnt_proc.c_str, "proc", 0, NULL) != 0) {
    fprintf(stderr, "Failed to mount loop device (%s) to mount point (%s).\n", "none", mnt_proc.c_str);
    fprintf(stderr, "Error (%d) %s\n", errno, strerror(errno));
    goto error;
  }

  if(mount("/sys", mnt_sys.c_str, NULL, MS_BIND|MS_REC, NULL) != 0) {
    fprintf(stderr, "Failed to mount loop device (%s) to mount point (%s).\n", "/sys", mnt_sys.c_str);
    fprintf(stderr, "Error (%d) %s\n", errno, strerror(errno));
    goto error;
  }

  if(drop()) goto error;

  free(boot_loop);
  free(root_loop);

  fstrfree(&mnt_boot);
  fstrfree(&mnt_dev);
  fstrfree(&mnt_proc);
  fstrfree(&mnt_sys);

  return 0;

  error:
    free(boot_loop);
    free(root_loop);

    fstrfree(&mnt_boot);
    fstrfree(&mnt_dev);
    fstrfree(&mnt_proc);
    fstrfree(&mnt_sys);

    /* TODO: Should clean up allocated loop devices too. */

    return 1;
}
