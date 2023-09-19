#include "paging.h"
#include "memory/heap/kheap.h"


void paging_load_directory(uint32_t* directory);

static uint32_t* current_directory= 0;

struct paging_4GB_chunk* paging_new_4GB(uint8_t flags)
{
    uint32_t* directory= kzalloc(sizeof(uint32_t)* PAGING_TOTAL_ENTRIES_PER_TABLE);
    //32bits (denoting page table)*1024 entry=size of page table directory.

    int offset=0;
    for(int i=0;i< PAGING_TOTAL_ENTRIES_PER_TABLE ;i++)
    {
        //initializing the page table directory's each entry, which contains address and flag of page table.
        //so create page table first.

        uint32_t* entry= kzalloc(sizeof(uint32_t)* PAGING_TOTAL_ENTRIES_PER_TABLE);
        //make page table =1024*32

        //now initialized each entry of pagetable, which will contain ram adrress and flag.
        for(int b=0; b< PAGING_TOTAL_ENTRIES_PER_TABLE; b++)
        {
            entry[b]= (offset + (b* PAGING_PAGE_SIZE)) | flags;//assigning address to entry in page table(actual ram address).

        }
        offset= offset +(PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);//1024*4096 for one table
        directory[i] =(uint32_t)entry |flags | PAGING_IS_WRITABLE;//as page table directory is writable, so, writable and page table is not writable so , no writable flag.
    }

    struct paging_4GB_chunk* chunk_4GB =kzalloc(sizeof(struct paging_4GB_chunk));
    chunk_4GB->directory_entry =directory;//contain base address of page table directory.
    //return page table directory address;

    return chunk_4GB;
}

void paging_switch(uint32_t* directory)
{   
    //put address of page table directory in cr3
    paging_load_directory(directory);
    current_directory =directory;
}

//return address of page directory table.
uint32_t* paging_4GB_chunk_get_directory(struct paging_4GB_chunk* chunk)
{
    return chunk->directory_entry;
}



