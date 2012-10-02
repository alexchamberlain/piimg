#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mount.h>

#include <piimg.h>
#include "builtin.h"

static void print_usage() {
  const char usage_string[] =
    "piimg umount <mount-point>";

  printf("Usage:\n\t%s\n", usage_string);
}

int ui_umount(const char * mnt) {
  int rc;

  if((rc = umount2(mnt, MNT_DETACH)) != 0) {
    fprintf(stderr, "Failed to unmount %s.\n", mnt);
    fprintf(stderr, "Error (%d) %s\n", errno, strerror(errno));
  }

  return rc;
}

int cmd_umount(int argc, char* argv[]) {
  char *boot_loop;
  char *root_loop;
  struct piimg_img simg;

  FSTR_DECLARE_WRAPPER(mnt_root, argv[0]);
  FSTR_DECLARE_NULL   (mnt_boot);
  FSTR_DECLARE_NULL   (mnt_dev);
  FSTR_DECLARE_NULL   (mnt_proc);
  FSTR_DECLARE_NULL   (mnt_sys);

  const FSTR_DECLARE_WRAPPER(dir_boot, "/boot");
  const FSTR_DECLARE_WRAPPER(dir_dev,  "/dev");
  const FSTR_DECLARE_WRAPPER(dir_proc, "/proc");
  const FSTR_DECLARE_WRAPPER(dir_sys,  "/sys");

  memset(&simg, 0, sizeof(struct piimg_img));

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

  if(ui_umount(mnt_sys.c_str)
    || ui_umount(mnt_proc.c_str)
    || ui_umount(mnt_boot.c_str)
    || ui_umount(mnt_dev.c_str)
    || ui_umount(mnt_root.c_str)) {
    goto error;
  }

  if(drop()) goto error;

  fstrfree(&mnt_boot);
  fstrfree(&mnt_dev);
  fstrfree(&mnt_proc);
  fstrfree(&mnt_sys);

  return 0;

  error:
    fstrfree(&mnt_boot);
    fstrfree(&mnt_dev);
    fstrfree(&mnt_proc);
    fstrfree(&mnt_sys);

    /* TODO: Should clean up allocated loop devices too. */

    return 1;
}


