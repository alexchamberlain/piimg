#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/loop.h>

#include "loopdev.h"
#include "privileges.h"

static const char LOOPDEV_PREFIX[]   = "/dev/loop";
static int        LOOPDEV_PREFIX_LEN = sizeof(LOOPDEV_PREFIX)/sizeof(LOOPDEV_PREFIX[0])-1;

char * loopdev_find_unused() {
  int control_fd = -1;
  int n = -1;

  if(escalate()) return NULL;

  if((control_fd = open("/dev/loop-control", O_RDWR)) < 0) {
    fprintf(stderr, "Failed to open /dev/loop-control\n");
    return NULL;
  }

  if(drop()) return NULL;

  n = ioctl(control_fd, LOOP_CTL_GET_FREE);

  if(n < 0) {
    fprintf(stderr, "Failed to find a free loop device.\n");
    return NULL;
  }
  
  int l = strlen(LOOPDEV_PREFIX) + 1 + 1; /* 1 for first character, 1 for NULL */
  {
    int m = n;
    while(m /= 10) {
      ++l;
    }
  }

  char * loopdev = (char*) malloc(l * sizeof(char));
  assert(sprintf(loopdev, "%s%d", LOOPDEV_PREFIX, n) == l - 1);

  return loopdev;
}

int loopdev_setup_device(const char * file, uint64_t offset, const char * device) {
  int file_fd = open(file, O_RDWR);
  int device_fd = -1;

  struct loop_info64 info;

  if(file_fd < 0) {
    fprintf(stderr, "Failed to open backing file (%s).\n", file);
    goto error;
  }

  if(escalate()) goto error;

  if((device_fd = open(device, O_RDWR)) < 0) {
    fprintf(stderr, "Failed to open device (%s).\n", device);
    goto error;
  }

  if(drop()) goto error;

  if(ioctl(device_fd, LOOP_SET_FD, file_fd) < 0) {
    fprintf(stderr, "Failed to set fd.\n");
    goto error;
  }

  close(file_fd);
  file_fd = -1;

  memset(&info, 0, sizeof(struct loop_info64)); /* Is this necessary? */
  info.lo_offset = offset;
  /* info.lo_sizelimit = 0 => max available */
  /* info.lo_encrypt_type = 0 => none */

  if(ioctl(device_fd, LOOP_SET_STATUS64, &info)) {
    fprintf(stderr, "Failed to set info.\n");
    goto error;
  }

  close(device_fd);
  device_fd = -1;

  return 0;

  error:
    if(file_fd >= 0) {
      close(file_fd);
    }
    if(device_fd >= 0) {
      ioctl(device_fd, LOOP_CLR_FD, 0);
      close(device_fd);
    }
    return 1;
}
