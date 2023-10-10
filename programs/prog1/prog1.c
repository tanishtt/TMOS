#include "function.h"
#include "stdlib.h"
#include "stdio.h"
#include <stdint.h>
#include "string.h"



int main(int argc, char** argv)
{
    print("from main...\n");
    //print(itoa(34657));

    //putchar('s');

    //int i=21;
    //printf("my age is %i\n", 21);


    // void* ptr =malloc(512);
    // if(ptr)
    // {
    //     print("allocated\n");
    // }
    // else{
    //     print("not allocated\n");
    // }
    // free(ptr);


    // sub_getkeyblock();
    // print("test abc");

    // char buff[1024];
    // sub_terminal_readline(buff, sizeof(buff), true);
    // printf("%s",buff);

    // char words[] = "this is tanish mohanta.";

    // const char* token = strtok(words, " ");
    // while(token)
    // {
    //     printf("%s\n", token);
    //     token = strtok(NULL, " ");
    // }

    //now user can access the memory
    // char* ptr=malloc(25);
    // strcpy(ptr, "hello sabhi log");
    // print(ptr);
    // free(ptr);


    // char str[] ="this is tanish.";
    // struct command_argument* root_command=sub_parse_command(str, sizeof(str));
    // printf("%s\n", root_command->argument);
    // printf("%s\n", root_command->next->argument);

    // struct process_arguments arguments;
    // sub_process_get_arguments(&arguments);
    // printf("%i %s\n", arguments.argc, arguments.argv[0]);

    //print(argv[0]);

    for (int i = 0; i <argc; i++)
    {
        printf("%s\n", argv[i]);
    }

    char* ptr = (char*)0x00;
    *ptr = 0x50;


    while(1){
        // if(getkey()!=0)
        // {
        //     print("other than 0x00 is pressed..\n");
        // }
    }
    return 0;
}