#include "memory.h"
/////////////////////////
void* memset(void* ptr, int ch, size_t size)
{
    char* ptr_ =(char*)ptr;
    for(int i=0; i<size; i++)
    {
        ptr_[i]=(char)(ch);
    }
    return ptr;
}