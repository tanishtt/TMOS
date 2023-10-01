#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "string/string.h"
#include "disk/streamer.h"
#include "fs/file.h"
#include "gdt/gdt.h"
#include "config.h"
#include "memory/memory.h"
#include "task/tss.h"



uint16_t* video_mem = NULL;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char ch, char colour)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(ch, colour);
}

void terminal_writechar(char ch, char colour)
{
    if (ch == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }
    
    terminal_putchar(terminal_col, terminal_row, ch, colour);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}
void terminal_initialize()
{
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }   
}


// size_t strlen(const char* str)
// {
//     size_t len = 0;
//     while(str[len])
//     {
//         len++;
//     }

//     return len;
// }

void print(const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}


static struct paging_4GB_chunk* kernel_chunk =0;

void panic(const char* msg)
{
    print(msg);
    while(1){}
}

struct tss tss;

struct gdt gdt_real[TMOS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[TMOS_TOTAL_GDT_SEGMENTS]={
    {.base =0x00, .limit =0x00, .type=0x00},       //null segment
    {.base =0x00, .limit =0xffffffff, .type=0x9a}, //kernel code segment
    {.base =0x00, .limit =0xffffffff, .type=0x92}, //kernel data segment
    {.base =0x00, .limit =0xffffffff, .type=0xf8}, //user code segment.
    {.base =0x00, .limit =0xffffffff, .type=0xf2}, //user data segment.
    {.base =(uint32_t)&tss, .limit =sizeof(tss), .type=0xe9} //tss segment.

};

void kernel_main()
{
    terminal_initialize();

    memset(gdt_real, 0x00, sizeof(gdt_real));
    gdt_structured_to_gdt(gdt_real, gdt_structured, TMOS_TOTAL_GDT_SEGMENTS);

    //loading the gdt.
    gdt_load(gdt_real, sizeof(gdt_real));

    kheap_init();
    print("\n->in kernel.c\n->heap initialized\n");

    //initialize filesystem.
    fs_init();

    //search and initialize the disk
    disk_search_and_init();

    
    idt_init();
    print("\n->in kernel.c\n->idt initialized\n");
    
    //set the tss.
    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 =0x600000;
    tss.ss0 =KERNEL_DATA_SELECTOR;

    //load the tss
    tss_load(0x28);//0x28 = offset in gdt real.
    

    kernel_chunk =paging_new_4GB(PAGING_IS_WRITABLE |PAGING_IS_PRESENT |PAGING_ACCESS_FROM_ALL);
    print("\n->in kernel.c\n->paging setup\n");
    //setting up paging

    paging_switch(kernel_chunk);
    //put address of page table directory table in cr3.
    //switch to kernel paging.

    enable_paging();
    //enable paging.

    //enable the system interrupts
    enable_interrupts();

int fd= fopen("0:/test.txt","r");
if(fd)
{
    print("we opened hello.txt file \n");
    char buff[30];
    fseek(fd, 2, SEEK_SET);
    fread(buff, 30, 1, fd);
    print(buff);

    // struct file_stat s;
    // fstat(fd,&s);
    fclose(fd);
}
else
{
    print("not able to open hello.txt \n");
}
while(1){}


    //just for testing...

//..............
    // struct path_root* root_path =pathparser_parse("0:/bin/shell.exe",NULL);

    // if(root_path){}

//..............
    // char* ptr= kzalloc(4096);
    // paging_set(paging_4GB_chunk_get_directory(kernel_chunk), (void*) 0x1000, (uint32_t)ptr |PAGING_ACCESS_FROM_ALL| PAGING_IS_WRITABLE| PAGING_WRITE_THROUGH);

    // char* ptr2 =(char*)0x1000;
    // ptr2[0]='a';
    // ptr2[1]='b';
    // print(ptr2);

    // print(ptr);

//..............
    // void* ptr=kmalloc(50);//0x1000000
    // void* ptr2=kmalloc(5000);//0x1001000
    // void* ptr3=kmalloc(5746);//0x1003000
    // kfree(ptr);
    // void* ptr4=kmalloc(34);//0x1000000
    // //testing...
    // if(ptr2||ptr||ptr3||ptr4)
    // {

    // }

}