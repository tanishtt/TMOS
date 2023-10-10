#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "function.h"


int main(int argc, char** argv)
{
    while(1)
    {
        print("TMOS-$ ");
        char buff[1024];
        sub_terminal_readline(buff, sizeof(buff), true);
        //sub_process_load_start(buff);
        print("\n");
        sub_system_run(buff);
        print("\n");
    }
    return 0;
}