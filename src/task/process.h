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
};

int process_load(const char* filename, struct process** process);
int process_load_for_slot(const char* filename, struct process** process, int process_slot);



#endif

