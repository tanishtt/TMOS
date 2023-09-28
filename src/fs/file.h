#ifndef FILE_H
#define FILE_H

#include "pparser.h"


typedef unsigned int FILE_SEEK_MODE;
enum
{
    SEEK_SET,
    SEE_CURR,
    SEEK_END
};


typedef unsigned int FILE_MODE;
enum
{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

struct disk;
//file open function.
typedef void*(*FS_OPEN_FUNCTION)(struct disk* disk, struct path_part* path, FILE_MODE mode);
typedef int (*FS_RESOLVE_FUNCTION)(struct disk* disk);


#endif