#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

typedef unsigned int DISK_TYPE;

//represent real physical harddisk
#define DISK_TYPE_REAL 0

//represent disk
struct disk
{
    DISK_TYPE type;
    int sector_size;
    //here file system functionality should also be there.
};

//int disk_read_sector(int lba, int total, void* buffer);
void disk_search_and_init();
struct disk* disk_get(int index);
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buff);


#endif