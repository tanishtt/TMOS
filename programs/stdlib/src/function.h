#ifndef FUNCTION_H
#define FUNCTION_H
#include <stddef.h>


void print(const char* message);
int getkey();
void* sub_malloc(size_t size);
void sub_free(void* ptr);
void sub_putchar(char c);
#endif