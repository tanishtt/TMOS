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

void memcmp(void* s1, void*s2, int count)
{
    char* c1 =s1;
    char* c2= s2;
    while(count-- >0)
    {
        if(*c1++ != *c2++)
        {
            return c1[-1] <c2[-1] ? -1 : 1;
        }
    }
    return 0;
}