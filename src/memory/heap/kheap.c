#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"


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
