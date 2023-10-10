#ifndef FUNCTION_H
#define FUNCTION_H
#include <stddef.h>
#include <stdbool.h>

struct command_argument
{
    char argument[512];
    struct command_argument* next;
};

struct process_arguments
{
    int argc;
    char** argv;
};

void print(const char* message);
int sub_getkey();
void* sub_malloc(size_t size);
void sub_free(void* ptr);
void sub_putchar(char c);
int sub_getkeyblock();
void sub_terminal_readline(char *out, int max, bool output_while_typing);
void sub_process_load_start(const char* filename);

void sub_process_get_arguments(struct process_arguments* arguments);


struct command_argument* sub_parse_command(const char* command, int max);

#endif