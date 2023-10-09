#include "string.h"

char tolower(char s1)
{
    if(s1>= 65 && s1<=90)
    {
        s1=s1+32;
    }
    return s1;
}

int strlen( const char* ptr)
{
    int i=0;
    while(*ptr!=0)
    {
        i++;
        ptr=ptr+1;
    }
    return i;
}

int strnlen( const char* ptr, int max)
{
    int i=0;
    for(i=0;i <max; i++)
    {
        if(ptr[i] ==0)
        {
            break;
        }
    }
    return i;
}

int strnlen_terminator(const char* str, int max, char terminator)
{//it will also terminate if it founds the terminator character.
    int i=0;
    for(i=0; i<max; i++)
    {
        if(str[i]=='\0' || str[i]==terminator)
        {
            break;
        }
    }
    return i;
}

char* strcpy(char* dest, const char* src)
{
    char *result=dest;
    while(*src !=0)
    {
        *dest =*src;
        src+=1;
        dest+=1;

    }
    *dest =0x00;
    return result;
}

char* strncpy(char* dest, const char* src, int count)
{
    int i = 0;
    for (i = 0; i < count-1; i++)
    {
        if (src[i] == 0x00)
            break;

        dest[i] = src[i];
    }

    dest[i] = 0x00;
    return dest;
}


int strncmp(const char* str1, const char* str2, int n)
{
    unsigned int u1,u2;

    while(n-- >0)
    {
        u1= (unsigned char)*str1++;
        u2= (unsigned char)*str2++;
        if(u1!= u2)
        {
            return u1- u2;
        }
        if(u1=='\0')
        {
            return 0;
        }
    }
    return 0;
}

int istrncmp(const char* s1, const char*s2, int n)
{//case insensitive.
    unsigned char u1,u2;
    while(n-- >0)
    {
        u1 =(unsigned char)*s1++;
        u2 =(unsigned char)*s2++;
        if(u1!=u2 && tolower(u1)!= tolower(u2))
        {
            return u1-u2;
        }
        if(u1=='\0')
        {
            return 0;
        }
    }
    return 0;
}


bool isdigit(char c)
{
    return c>=48 && c<=57;
}
int converttonumeric(char c)
{
    return c-48;
}

// char s[] = "This is the way again."; char d[] = "";
// char *portion = strtok(s, d);
// while (portion != NULL)
// {
//      printf("%s\n", portion); 
//      portion = strtok (NULL, d);
// }

//but it add '\0' after each delimiter.
//s[] -> This\0is\0the\0way\0again.\0\0;


char* nextToken = 0;
char* strtok(char* str, const char* delimiters)
{
    int i = 0;
    int len = strlen(delimiters);

    if (!str && !nextToken)
    {
        return 0;
    }

    if (str && !nextToken)
    {
        nextToken = str;
    }

    char* tokenStart = nextToken;
    while (1)
    {
        for (i = 0; i < len; i++)
        { //check with all delimiter.
            if (*tokenStart == delimiters[i])
            {
                tokenStart++;
                break;
            }
        }

        if (i == len)
        { //means that's a character. let's say 'T'
            nextToken = tokenStart;
            break;
        }
    }

    if (*nextToken == '\0')
    {
        nextToken = 0;
        return nextToken;
    }

    //find end of substring
    while (*nextToken != '\0')
    {
        for (i = 0; i < len; i++)
        {
            if (*nextToken == delimiters[i])
            {
                *nextToken = '\0';
                break;
            }
        }

        nextToken++; //one by one char, at the end of the next word.
        if (i < len)
            break;
    }

    return tokenStart; //This\0
}
