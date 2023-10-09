#ifndef STDLIB_STRING_H
#define STDLIB_STRING_H

#include <stdbool.h>

char tolower(char s1);

int strlen( const char* ptr);

int strnlen( const char* ptr, int max);

int strnlen_terminator(const char* str, int max, char terminator);

char* strcpy(char* dest, const char* src);

char* strncpy(char* dest, const char* src, int count);


int strncmp(const char* str1, const char* str2, int n);

int istrncmp(const char* s1, const char*s2, int n);


bool isdigit(char c);
int converttonumeric(char c);

char* strtok(char* str, const char* delimiters);

#endif