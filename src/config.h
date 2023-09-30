#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0X08
#define KERNEL_DATA_SELECTOR 0X10


//minimum 256 maximum 512
#define TOTAL_INTERRUPTS 512

#define HEAP_SIZE_BYTES 104857600//100mb
#define HEAP_BLOCK_SIZE 4096
#define HEAP_ADDRESS 0x01000000
#define HEAP_TABLE_ADDRESS 0x00007E00

#define SECTOR_SIZE 512

#define MAX_FILESYSTEMS 12
#define MAX_FILE_DESCRIPTORS 512 
//max file open.

#define MAX_PATH 108//max file length.

#define TMOS_TOTAL_GDT_SEGMENTS 6

#endif