#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#define TMOS_MAX_PATH 108//max length for file access.

void kernel_main();
void print(const char* str);
void panic(const char* msg);
void terminal_writechar(char ch, char colour);

void kernel_page();
void kernel_registers();


#define ERROR(value) (void*)(value)
#define ERROR_I(value) (int)(value)
#define ISERR(value) ((int) value <0)


#endif