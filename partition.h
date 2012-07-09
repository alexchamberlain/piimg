#include <stdint.h>

struct piimg_img {
  uint64_t size;
  uint64_t boot_offset;
  uint64_t root_offset;
};

int analyse_img(struct piimg_img *, const char *);
