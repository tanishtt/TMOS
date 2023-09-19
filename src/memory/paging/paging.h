#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define PAGING_CACHE_DISABLED   0b00010000
#define PAGING_WRITE_THROUGH    0b00001000
#define PAGING_ACCESS_FROM_ALL  0b00000100
#define PAGING_IS_WRITABLE      0b00000010
#define PAGING_IS_PRESENT       0b00000001
 
#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024// for both page directory and page table.
#define PAGING_PAGE_SIZE 4096 //actual ram page size(bytes).

//page directory here, which will map 4gb address space.
struct paging_4GB_chunk
{
    uint32_t* directory_entry;
};

struct paging_4GB_chunk* paging_new_4GB(uint8_t flags);
uint32_t* paging_4GB_chunk_get_directory(struct paging_4GB_chunk* chunk);
void paging_switch(uint32_t* directory);
void enable_paging();


bool paging_is_aligned(void* addr);
int paging_set(uint32_t* directory, void* virtAddr, uint32_t val);


#endif