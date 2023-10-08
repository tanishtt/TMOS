#include "stdlib.h"
#include "function.h"

void* malloc(size_t size)
{
    return sub_malloc(size);
}

void free(void* ptr)
{
    sub_free(ptr);
}