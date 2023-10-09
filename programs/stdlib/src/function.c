#include "function.h"
#include "string.h"

struct command_argument* sub_parse_command(const char* command, int max)
{
    //find -name "query"
    //find(root node) -> -name -> "query"
    struct command_argument* root_command =0;
    char scommand[1024];
    if (max >=(int)sizeof(scommand))
    {
        return 0;
    }


    strncpy(scommand, command, sizeof(scommand));
    char* token =strtok(scommand, " ");
    if (!token)
    {
        goto out;
    }

    root_command =sub_malloc(sizeof(struct command_argument));
    if (!root_command)
    {
        goto out;
    }

    strncpy(root_command->argument, token, sizeof(root_command->argument));
    root_command->next =0;


    struct command_argument* current =root_command;
    token = strtok(NULL, " ");
    while(token !=0)
    {
        struct command_argument* new_command =sub_malloc(sizeof(struct command_argument));
        if (!new_command)
        {
            break;
        }

        strncpy(new_command->argument, token, sizeof(new_command->argument));
        new_command->next =0x00;
        current->next =new_command;
        current =new_command;
        token =strtok(NULL, " ");
    }
    //find -name "query"
    //find(root node) -> -name -> "query"
out:
    return root_command;
}


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
