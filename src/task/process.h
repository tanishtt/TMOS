#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#include "config.h"

struct process
{
    //process id.
    uint16_t id;

    char filename[MAX_PATH];

    //main process task.
    struct task* task;

    //THE MEMORY(MALLOC) ALLOCATION OF THE PROCESS.
    void* allocation[MAX_PROGRAM_ALLOCATIONS];
    //KEEP TRACK OF ALL THE ALLOCATION, SO THAT AT LAST WE CAN CHECK WHETHER WE HAVE FREED ALL OF THEM.

    //PHYSICAL pointer to the process.
    void* ptr;

    //physical pointer to stack memory.
    void* stack;

    //size of the data pointed to by pointer "ptr".
    uint32_t size;

    struct keyboard_buffer
    {
        char buffer[TMOS_KEYBOARD_BUFFER_SIZE];
        int tail;
        int head;

    }keyboard;
    
};

int process_load(const char* filename, struct process** process);
int process_load_for_slot(const char* filename, struct process** process, int process_slot);
struct process *process_current();
struct process *process_get(int process_id);

#endif

