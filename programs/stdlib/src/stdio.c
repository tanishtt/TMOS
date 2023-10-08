#include "stdio.h"
#include "function.h"
#include <stdarg.h>
#include "stdlib.h"


int putchar(int ch)
{
    sub_putchar((char)ch);
    return 0;
}

//example for stdarg.h
// int function(int num_args, ...)
// {
//     va_list args;
//     va_start(args, num_args);//after num_args
//     for (int i = 0; i < num_args; i++)
//     {
//     int x = va_arg(args, int);
//     printf("x: %d\n", x);
//     }
//     va_end (args);
//     return 0;
// }

int printf(const char* format, ...)
{
    va_list ap;
    const char *ptr;
    char *sval;
    int ival;

    va_start(ap, format);
    for(ptr=format; *ptr; ptr++)
    {
        if(*ptr!= '%')
        {
            putchar(*ptr);
            continue;
        }
        //if %
        switch(*++ptr)
        {
            case 'i'://if i
            ival= va_arg(ap,int);
            print(itoa(ival));
            break;

            case 's':
            sval =va_arg(ap, char*);
            print(sval);
            break;

            default:
            putchar(*ptr);
            break;
        }
    }

    va_end(ap);
    return 0;
}