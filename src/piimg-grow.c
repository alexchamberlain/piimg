#include <stdio.h>
#include <fcntl.h>

#include <piimg.h>
#include "builtin.h"

static void print_usage() {
  const char usage_string[] =
    "piimg grow <img-file> <sd-card>\n"
  "\t  Grows an image file to the same size as an <sd-card>.";

  printf("Usage:\n\t%s\n", usage_string);
}

int cmd_grow(int argc, char* argv[]) {
  struct piimg_img sd_card;
  struct piimg_img img;
  int img_fd = -1;

  if(analyse_device(&sd_card, argv[1])) {
    fprintf(stderr, "Failed to analyse device (%s).\n", argv[1]);
    goto error;
  }
  
  if((img_fd = open(argv[0], O_RDWR)) < 0) {
    fprintf(stderr, "Failed to open image file (%s).\n", argv[0]);
    goto error;
  }

  if(analyse_img(&img, argv[0])) {
    fprintf(stderr, "Failed to analyse image file (%s).\n", argv[0]);
  }

  if(img.size > sd_card.size) {
    fprintf(stderr, "WARNING: The image file is being shrunk.\n");
  }

  if(img.size != sd_card.size) {
    /* Only resize if we have to! */
    if(ftruncate(img_fd, sd_card.size) < 0) {
      fprintf(stderr, "Failed to resize image file.\n");
      goto error;
    }
  }

  close(img_fd);

  return 0;

  error:
    if(img_fd >= 0) {
      close(img_fd);
    }

    return 1;
}
