#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;
//100 mb=104857600 bytes.
//104857600/4096=25600 entries
void kheap_init()
{
    int total_table_entries = HEAP_SIZE_BYTES/ HEAP_BLOCK_SIZE;
    kernel_heap_table.entries=(HEAP_BLOCK_TABLE_ENTRY*)(HEAP_TABLE_ADDRESS);
    kernel_heap_table.total=total_table_entries;
    void* end=(void*)(HEAP_ADDRESS + HEAP_SIZE_BYTES);
    int res= heap_create(&kernel_heap,(void*)(HEAP_ADDRESS), end, &kernel_heap_table);
    if(res<0)
    {
        //error
        print("failed in creating heap\n");
    }
}

void* kmalloc(size_t size)
{
    return heap_malloc(&kernel_heap, size);
}
//malloc+ initialized to 0
void* kzalloc(size_t size)
{
    void* ptr= kmalloc(size);
    if(!ptr)
    {
        return 0;
    }
    memset(ptr,0x00,size);
    return ptr;
}

void kfree(void* ptr)
{
    return heap_free(&kernel_heap, ptr);
}




