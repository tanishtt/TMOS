#include "pparser.h"
#include "string/string.h"
#include "kernel.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"



static int path_valid_format(const char*filename)
{
    int len= strnlen(filename, TMOS_MAX_PATH);

    return (len>=3 && isdigit(filename[0]) && memcmp((void*)&filename[1], ":/", 2)== 0);//check 2 bytes":/" from address of filename[1] is same or not.
}