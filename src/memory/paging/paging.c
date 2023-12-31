#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"
//#include "kernel.h"

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

void paging_switch(struct paging_4GB_chunk* directory)
{   //print("paging_switch\n");
    //put address of page table directory in cr3
    paging_load_directory(directory->directory_entry);
    current_directory =directory->directory_entry;
}

void paging_free_4GB(struct paging_4GB_chunk* chunk)
{
    //free all the page directory entry.
    for(int i=0;i< 1024; i++)
    {
        uint32_t entry =chunk->directory_entry[i];
        uint32_t* table =(uint32_t*)(entry &0xfffff000);

        kfree(table);
    }
    //finally delete the 4gb chunk.
    kfree(chunk->directory_entry);
    kfree(chunk);

}

//return address of page directory table.
uint32_t* paging_4GB_chunk_get_directory(struct paging_4GB_chunk* chunk)
{
    return chunk->directory_entry;
}
///////
bool paging_is_aligned(void* addr)
{
    return((uint32_t)addr % PAGING_PAGE_SIZE)==0;
}
int paging_get_indexes(void* virtual_address, uint32_t* directory_index_out, uint32_t* table_index_out)
{
    int res=0;
    if(!paging_is_aligned(virtual_address))
    {
        res =-EINVARG;
        goto out;
    }
    //taking virtual address and calculating which directory index(page table) and in that page table which index(address in ram).

    //calculating the index in page table directory table.
    *directory_index_out =((uint32_t)virtual_address /(PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
    //calculating the index in a page table(which will give address in ram).
    *table_index_out = (((uint32_t) virtual_address %(PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE))/ PAGING_PAGE_SIZE);
    
out:
    return res;
}

int paging_set(uint32_t* directory, void* virtAddr, uint32_t val)
{
    if(!paging_is_aligned(virtAddr))
    {
        return -EINVARG;
    }

    uint32_t directory_index= 0;
    uint32_t table_index= 0;
    int res= paging_get_indexes(virtAddr, &directory_index, &table_index);
    if(res< 0)
    {
        return res;
    }

    uint32_t entry =directory[directory_index];
    uint32_t* table= (uint32_t*)(entry & 0xfffff000);//get the first 20 bits which contains the address of page table.
    table[table_index] =val;//val=physical address+flags(32 bit).

    return 0;
}

void* paging_align_address(void* ptr)
{
    if ((uint32_t)ptr % PAGING_PAGE_SIZE)
    {
        return (void*)((uint32_t)ptr + PAGING_PAGE_SIZE - ((uint32_t)ptr % PAGING_PAGE_SIZE));
    }

    return ptr;
}

void* paging_align_to_lower_page(void* vaddr)
{
    uint32_t addr= (uint32_t) vaddr;
    addr -=(addr %PAGING_PAGE_SIZE);
    return (void*) addr;
    
}

int paging_map(struct paging_4GB_chunk* directory, void* virt, void* phys, int flags)
{
    if(((unsigned int)virt %PAGING_PAGE_SIZE) || ((unsigned int)phys % PAGING_PAGE_SIZE))
    {
        return -EINVARG;

    }
    return paging_set(directory->directory_entry, virt, (uint32_t)phys|flags);
    
}

int paging_map_range(struct paging_4GB_chunk* directory, void* virt, void* phys, int count, int flags)
{
   int res=0;
   for(int i=0;i< count;i++)
   {
        res =paging_map(directory, virt, phys, flags);
        if(res <0)
        {
            break;
        }
        virt+= PAGING_PAGE_SIZE;
        phys+= PAGING_PAGE_SIZE;
   }
   return res;
}

int paging_map_to(struct paging_4GB_chunk* directory, void* virt, void* phys, void* phys_end, int flags)
{
    int res=0;
    if((uint32_t)virt% PAGING_PAGE_SIZE)
    {
        res =-EINVARG;
        goto out;
    }
    if((uint32_t)phys % PAGING_PAGE_SIZE)
    {
        res =-EINVARG;
        goto out;
    }
    if((uint32_t)phys_end % PAGING_PAGE_SIZE)
    {
        res =-EINVARG;
        goto out;
    }
    if((uint32_t)phys_end < (uint32_t)phys)
    {
        res =-EINVARG;
        goto out;
    }

    uint32_t total_bytes =phys_end- phys;
    int total_pages =total_bytes/ PAGING_PAGE_SIZE;
    res =paging_map_range(directory, virt, phys, total_pages, flags);


out:
    return res;
}



void* paging_get_physical_address(uint32_t* directory, void* virt)
{
    void* virt_addr_new = (void*) paging_align_to_lower_page(virt);
    void* difference = (void*)((uint32_t) virt - (uint32_t) virt_addr_new);
    return (void*)((paging_get(directory, virt_addr_new) & 0xfffff000) + difference);
}


//take the virt address and grab that actual entry.

uint32_t paging_get(uint32_t* directory, void* virt)
{
    uint32_t directory_index =0;
    uint32_t table_index=0;
    paging_get_indexes(virt, &directory_index, &table_index);
    uint32_t entry = directory[directory_index];
    uint32_t* table =(uint32_t*)(entry & 0xfffff000);
    return table[table_index];

}
