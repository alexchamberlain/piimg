#include <parted/parted.h>

#include "partition.h"

int analyse_img(struct piimg_img * simg, const char * img) {
  PedDevice* device;
  PedDisk* disk;
  PedPartition* part = NULL;

  device = ped_device_get(img);
  if (device == NULL)
    goto error;

  disk = ped_disk_new(device);
  if(disk == NULL)
    goto error_device_destroy;

  /* We shall set the first fat16 partition to be /boot and the first ext4 partition to be / */

  if(device->type != 5)
    fprintf(stderr, "WARNING: The device being mounted is not an image file.\n");

  simg->size = device->length * device->sector_size;

  while(part = (PedPartition*) ped_disk_next_partition(disk, part)) {
    uint64_t offset;
    if(part->num < 0)
      continue;

    offset = device->sector_size * part->geom.start;

    if(simg->boot_offset == 0 && part->fs_type && (!strcmp("fat16", part->fs_type->name) || !strcmp("fat32", part->fs_type->name))) {
      simg->boot_offset = offset;
    }

    if(simg->root_offset == 0 && part->fs_type && !strcmp("ext4", part->fs_type->name)) {
      simg->root_offset = offset;
    }
  }

  ped_disk_destroy(disk);
  ped_device_destroy(device);

  return 0;

  error_disk_destroy:
    ped_disk_destroy(disk);
  error_device_destroy:
    ped_device_destroy(device);
  error:
    return 1;
}
