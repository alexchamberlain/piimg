#include <stdio.h>
#include <parted/parted.h>

#include "builtin.h"

static void print_usage() {
  const char usage_string[] =
    "piimg list {<img-file>|<SD card device>}";

  printf("Usage:\n\t%s\n", usage_string);
}


int cmd_list (int argc, char* argv[]) {
  char* size;
  char* transport[13] = {"unknown", "scsi", "ide", "dac960", "cpqarray", "file", "ataraid", "i2o", "ubd", "dasd", "viodasd", "sx8", "dm"};
  PedDevice* device;
  PedDisk* disk;
  PedPartition* part;

  if(argc != 1) {
    print_usage();
    return 1;
  }

  device = ped_device_get (argv[0]);
  if (device == NULL)
    goto error;

  size = ped_unit_format_byte (device, device->length * device->sector_size);
  if (size == NULL)
    goto error_device_destroy;

  disk = ped_disk_new(device);
  if(disk == NULL)
    goto error_free_size;

  printf ("Model: %s\n", device->model);
  printf ("Transport: %s\n", transport[device->type]);
  printf ("Size: %s\n", size);
  printf ("Sector size (logical/physical): %lldB/%lldB\n", device->sector_size, device->phys_sector_size);

  printf("no.\tstart\tsize\tend\toffset\tfs\n");

  for(part = (PedPartition*) ped_disk_next_partition(disk, NULL); part;
      part = (PedPartition*) ped_disk_next_partition(disk, part)) {
    long long offset;
    if(part->num < 0)
      continue;
    offset = device->sector_size * part->geom.start;
    printf("%d\t%lld\t%lld\t%lld\t%lld\t%s\n", part->num, part->geom.start, part->geom.length, part->geom.end, offset, (part->fs_type) ? part->fs_type->name : "");
  }

  ped_disk_destroy(disk);
  free (size);
  ped_device_destroy(device);

  return 0;

  error_disk_destroy:
    ped_disk_destroy(disk);
  error_free_size:
    free(size);
  error_device_destroy:
    ped_device_destroy(device);
  error:
    return 1;
}

