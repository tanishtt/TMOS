#include "function.h"


int sub_getkeyblock()
{
    int val=sub_getkey();
    do
    {
        val=sub_getkey();
    }while(val== 0);

    return val;
}

//wait until press enter and fill out buffer.
void sub_terminal_readline(char *out, int max, bool output_while_typing)
{
    int i =0;
    for (i =0; i <max -1; i++)
    {
        char key = sub_getkeyblock();

        //carriage return means we have hit enter
        if (key ==13)
        {
            break;
        }

        if (output_while_typing)
        {
            sub_putchar(key);
        }

        //backspace
        if (key == 0x08 && i >= 1)
        {
            out[i-1] = 0x00;
            //-2 because it will +1 when we go to continue i++
            i -=2;
            continue;
        }

        out[i] = key;
    }

    //add the null terminator
    out[i] = 0x00;

}
