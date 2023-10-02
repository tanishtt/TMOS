#include "misc.h"
#include "idt/idt.h"
#include "task/task.h"


void* isr80h_command0_sum(struct interrupt_frame* frame)
{//ex- command 0, summing two numbers together.
    int val2 =(int)task_get_stack_item(task_current(), 1);
    int val1 =(int)task_get_stack_item(task_current(), 0);
    return (void*)(val1+val2);
}