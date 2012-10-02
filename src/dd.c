#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int dd(int in_fd, int out_fd, size_t buffersize) {
  char * buffer;
  size_t pagesize = sysconf(_SC_PAGESIZE);
  size_t count;

  if(posix_memalign((void**) &buffer, pagesize, buffersize) != 0 || buffer == NULL) {
    fprintf(stderr, "Failed to allocate buffer.\n");
    return 1;
  }

  memset(buffer, 0, buffersize);
  while((count = read(in_fd, buffer, buffersize)) != 0) {
    if(write(out_fd, buffer, count) < count) {
      fprintf(stderr, "Failed to write the buffer to output.\n");
      goto error;
    }
    memset(buffer, 0, buffersize);
  }

  free(buffer);

  return 0;

  error:
    if(buffer != NULL) {
      /* Clear buffer to avoid any security problems */
      memset(buffer, 0, buffersize);
      free(buffer);
    }
    return 1;
}
