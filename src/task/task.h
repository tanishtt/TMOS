#ifndef TASK_H
#define TASK_H

#include "config.h"
#include "memory/paging/paging.h"
#include <stdint.h>

struct interrupt_frame;

struct registers
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    //
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t esp;
    uint32_t ss;
};


struct process;

//reopresent a task;
struct task
{
    //page directory for the task.
    struct paging_4GB_chunk* page_directory;

    //registers of the task when the task is not running.
    struct registers registers;

    //task of the process.
    struct process* process;

    //next task in the linked list(queue)
    struct task* next;

    //previous task in the linked list
    struct task* prev;

};

struct task* task_new(struct process* process);
struct task* task_current();
int task_free(struct task* task);
struct task* task_get_next();


int task_switch(struct task* task);

int task_page();
int task_page_task(struct task* task);


void task_run_first_ever_task();

void task_return(struct registers* regs);
void restore_general_purpose_registers(struct registers* regs);
void user_registers();

void task_current_save_state(struct interrupt_frame* frame);
int copy_string_from_task(struct task* task, void* virtual, void* phys, int max);
void* task_get_stack_item(struct task* task, int index);

#endif