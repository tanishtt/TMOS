#ifndef FUNCTION_H
#define FUNCTION_H
#include <stddef.h>
#include <stdbool.h>


void print(const char* message);
int sub_getkey();
void* sub_malloc(size_t size);
void sub_free(void* ptr);
void sub_putchar(char c);
int sub_getkeyblock();
void sub_terminal_readline(char *out, int max, bool output_while_typing);


#endif