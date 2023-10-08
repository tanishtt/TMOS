#include "function.h"
#include "stdlib.h"


int main(int argc, char** argv)
{
    print("from main...\n");
    void* ptr =malloc(512);
    if(ptr)
    {
        print("allocated\n");
    }
    else{
        print("not allocated\n");
    }
    free(ptr);
    while(1){
        if(getkey()!=0)
        {
            print("other than 0x00 is pressed..\n");
        }
    }
    return 0;
}