#include "heap.h"
#include "kernel.h"
#include "status.h"
#include <stdbool.h>
#include "memory/memory.h"

//all these validate function are validating that bytes and block are equal to that we provided in config.h (constant values), means everything should be match and only 100 mb and other constant. 
static int heap_validate_table(void* ptr, void* end, struct heap_table* table)
{
    int res=0;
    size_t table_size=(size_t)(end-ptr);
    size_t total_blocks= table_size / HEAP_BLOCK_SIZE;

    if(table->total != total_blocks)
    {
        res=-EINVARG;
        goto out;
    }
out:
    return res;
}

static bool heap_validate_alignment(void* ptr)
{
    return ((unsigned int)ptr% HEAP_BLOCK_SIZE)==0;
}

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)
{   //heap = heap area started...
    //ptr = (actual address)starting heap data pool pointer
    //end= end of heap.
    int res=0;
    if(!heap_validate_alignment(ptr)||!heap_validate_alignment(end))
    {
        res=-EINVARG;
        goto out;
    }

    memset(heap, 0, sizeof(struct heap));
    heap->saddr=ptr;
    heap->table=table;

    res= heap_validate_table(ptr, end, table);
    if(res<0)
    {
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY)*table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
    return res;
    //return 0 and negative values.
    //if 0 , created successfully, if not then neg. value represent the error code.
}

//functions to calculate number of blocks.
static uint32_t heap_align_value_to_upper(uint32_t val)
{
    if((val% HEAP_BLOCK_SIZE)== 0)
    {
        return val;
    }
    val =(val -(val% HEAP_BLOCK_SIZE))+ HEAP_BLOCK_SIZE;
    return val;
}
//now all function will complete heap_malloc_blocks.

static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry)
{
    return entry & 0x0f;
}

int heap_get_start_block(struct heap* heap, uint32_t total_blocks)
{
    struct heap_table* table= heap->table;

    int block_start=-1;
    int block_current=0;

    for(size_t i=0; i< table->total; i++)
    {
        if(heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE)
        {
            block_current=0;
            block_start=-1;
            continue;
        }

        //means the block is free
        //if this is first block
        if(block_start==-1)
        {
            block_start= i;
            //index in heap entry table
        }
        block_current++;

        if(block_current== total_blocks)
        {
            break;
        }
    }

    if(block_start ==-1)
    {
        return -ENOMEM;
    }
    return block_start;
}

void* heap_block_to_address( struct heap* heap, uint32_t block)
{
    //absolute address;
    return heap->saddr + (block * HEAP_BLOCK_SIZE);
}

void heap_mark_blocks_taken(struct heap* heap, int start_block, int total_blocks)
{
    int end_block= (start_block +total_blocks)-1;//end=0+4-1=3
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST; //8 bit entry.
    if( total_blocks>1)
    {
        entry = entry | HEAP_BLOCK_HAS_NEXT;
    }

    for(int i=start_block; i<=end_block; i++)
    {
        heap->table->entries[i] =entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if( i != end_block-1)
        {
            entry =entry | HEAP_BLOCK_HAS_NEXT;
        }
    }
}

//heap malloc block , takes number of blocks to allocate in memory.
void* heap_malloc_blocks(struct heap* heap, uint32_t total_blocks)
{
    void* address=0;

    //this function will search whether there is series of blocks free or not in table(means indirectly in heap).
    //if yes then return the start address of free array location.
    //will return index.
    int start_block= heap_get_start_block(heap, total_blocks);
    if(start_block< 0)
    {
        goto out;
    }
    //convert block to address(address )
    address = heap_block_to_address(heap, start_block);//calculate absolute address

    //mark the blocks taken
    heap_mark_blocks_taken(heap, start_block, total_blocks);

out:
    return address;
}
//heap malloc, takes number of bytes(size).
void* heap_malloc(struct heap* heap, size_t size)
{
    size_t aligned_size= heap_align_value_to_upper(size);
    uint32_t total_blocks = aligned_size/ HEAP_BLOCK_SIZE;

    return heap_malloc_blocks(heap, total_blocks);   
}
//heap free
void heap_free(struct heap* heap,void* ptr)
{
    
}

