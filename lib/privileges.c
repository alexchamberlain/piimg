#include <stdio.h>
#include <unistd.h>


int escalate() {
  if(seteuid(0) == -1 || geteuid() != 0) {
    fprintf(stderr, "Failed to escalate privileges.\n");
    return 1;
  }

  return 0;
}

int drop() {
  if(seteuid(getuid()) == -1 || geteuid() != getuid()) {
    fprintf(stderr, "Failed to drop privileges.\n");
    return 1;
  }

  return 0;
}
