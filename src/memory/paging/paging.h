#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>

#define PAGING_CACHE_DISABLED   0b00010000
#define PAGING_WRITE_THROUGH    0b00001000
#define PAGING_ACCESS_FROM_ALL  0b00000100
#define PAGING_IS_WRITABLE      0b00000010
#define PAGING_IS_PRESENT       0b00000001
 
#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024// for both page directory and page table.


//page directory here, which will map 4gb address space.
struct paging_4GB_chunk
{
    uint32_t* directory_entry;
};

#endif