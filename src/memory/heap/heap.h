#ifndef HEAP_H
#define HEAP_H
#include "config.h"
#include<stdint.h>
#include<stddef.h>


#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FIRST 0b01000000



#endif
