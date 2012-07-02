#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <linux/loop.h>

#include "loopdev.h"

static const char * LOOPDEV_PREFIX     = "/dev/loop";
static int          LOOPDEV_PREFIX_LEN = strlen("/dev/loop");

char * loopdev_find_unused() {
  int control_fd = open("/dev/loop-control", O_RDWR);
  int n = -1;

  if(control_fd < 0) {
    fprintf(stderr, "Failed to open /dev/loop-control\n");
    return NULL;
  }

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
