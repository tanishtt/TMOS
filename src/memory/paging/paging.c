#include "paging.h"
#include "memory/heap/kheap.h"

struct paging_4GB_chunk* paging_new_4GB(uint8_t flags)
{
    uint32_t* directory= kzalloc(sizeof(uint32_t)* PAGING_TOTAL_ENTRIES_PER_TABLE);//32bits (denoting page table)*1024 entry=size of page table directory.
}



