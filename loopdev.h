#include <stdint.h>

char * loopdev_find_unused();
int loopdev_setup_device(const char * file, uint64_t offset, const char * device);
