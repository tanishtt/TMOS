#include "stdlib.h"
#include "function.h"


char* itoa(int i)
{//integer to string.
    static char text[12];

    int loc=11;
    text[11]=0;
    char neg=1;
    if(i>=0)
    {
        neg=0;
        i=-i;
    }//if the input integer i is negative, there's no need to negate it again because it's already negative, and the code will correctly handle it as such.

    while(i)
    {
        text[--loc] ='0' -(i%10);
        i /=10;
    }
    if(loc ==11)
    {
        text[--loc] ='0';
    }
    if(neg)
    {
        text[--loc] ='-';
    }
    return &text[loc];
}

void* malloc(size_t size)
{
    return sub_malloc(size);
}

void free(void* ptr)
{
    sub_free(ptr);
}