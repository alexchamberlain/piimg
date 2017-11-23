#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <piimg.h>
#include "builtin.h"

static void print_usage() {
  const char usage_string[] =
    "piimg dd <img-file> <device-file>";

  printf("Usage:\n\t%s\n", usage_string);
}

int cmd_dd(int argc, char* argv[]) {
  int in_fd;
  int out_fd;

  if(escalate()) goto error;

  in_fd  = open(argv[0], O_RDONLY);
  out_fd = open(argv[1], O_WRONLY);

  if(drop()) goto error;

  if(in_fd < 0 || out_fd < 0) {
    fprintf(stderr, "Failed to open one of the files.\n");
    goto error;
  }
  
  if(dd(in_fd, out_fd, 4194304) != 0) {
    fprintf(stderr, "dd failed.\n");
    goto error;
  }

  return 0;

  error:
    if(in_fd > 0)
      close(in_fd);
    if(out_fd > 0)
      close(out_fd);

    return 1;
}
