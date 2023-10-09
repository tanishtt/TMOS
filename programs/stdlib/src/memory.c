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

int memcmp(void* s1, void*s2, int count)
{
    char* c1 =s1;
    char* c2= s2;
    while(count-- >0)
    {
        if(*c1++ != *c2++)
        {
            //if not equal, the point at which they are different.
            //-1 s1 is smaller than s2.
            //1 s2 is smaller than s1.
            return c1[-1] < c2[-1] ? -1 : 1;
        }
    }
    return 0;
}

void* memcpy(void* dest, void* src, int len)
{
    char* d=dest;
    char* s=src;
    while(len--)
    {
        *d++ = *s++;
    }
    return dest;
}