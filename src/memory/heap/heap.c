#include "heap.h"
#include "kernel.h"
#include <stdbool.h>


static int heap_validate_alignment(void* ptr)
{
    return ((unsigned int)ptr% HEAP_BLOCK_SIZE)==0;
}

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)
{//ptr = heap data pool pointer
//end= end of heap.
    return 0;
    //return 0 and negative values.
    //if 0 , created successfully, if not then neg. value represent the error code.
}