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

#define MAX_PROGRAM_ALLOCATIONS 1024
#define MAX_PROCESSES 12

#define TMOS_TOTAL_GDT_SEGMENTS 6

#define TMOS_PROGRAM_VIRTUAL_ADDRESS 0x400000//entry for program(user land).
#define TMOS_USER_PROGRAM_STACK_SIZE 1024* 16
#define TMOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START 0x3ff000
#define TMOS_PROGRAM_VIRTUAL_STACK_ADDRESS_END TMOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START -TMOS_USER_PROGRAM_STACK_SIZE

#define USER_DATA_SEGMENT 0x23
#define USER_CODE_SEGMENT 0x1b

#define TMOS_MAX_ISR80H_COMMANDS 1024

#define TMOS_KEYBOARD_BUFFER_SIZE 1024


#endif