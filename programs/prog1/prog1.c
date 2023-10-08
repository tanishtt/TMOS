#include "function.h"

int main(int argc, char** argv)
{
    print("from main...\n");
    while(1){
        if(getkey()!=0)
        {
            print("other than 0x00 is pressed..\n");
        }
    }
    return 0;
}