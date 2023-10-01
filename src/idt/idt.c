#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"
#include "task/task.h"

struct idt_descriptor idt_decriptors[TOTAL_INTERRUPTS];
struct idtr_ idtr;

static ISR80H_COMMAND isr80h_commands[TMOS_MAX_ISR80H_COMMANDS];

extern void idt_load(struct idtr_* ptr);
extern void int21h();
extern void no_interrupt();
extern void isr80h_wrapper();

/////////
void idt_zero()
{
    print("Oops!!, Divide by zero error.\n");
}
void int21h_handler()
{
    print("Keyboard pressed...");
    outb(0x20,0x20);
}
void no_interrupt_handler()
{
    outb(0x20,0x20);
}
//////////
void  idt_set(int interrupt_no, void* address)
{
    struct idt_descriptor *idt_ptr =&idt_decriptors[interrupt_no];
    idt_ptr->offset_1=(uint32_t)address & 0x0000ffff;//last 16 bits -low bits
    idt_ptr->selector= KERNEL_CODE_SELECTOR;
    idt_ptr->zero= 0x00;
    idt_ptr->type_attributes=0xEE;//1-11-0-1110
    idt_ptr->offset_2= (uint32_t) address >> 16; //high bits
}
void idt_init()
{
    memset(idt_decriptors, 0, sizeof(idt_decriptors));//all 512 decriptors are set to 0.

    idtr.limit = sizeof(idt_decriptors)-1;
    idtr.base =(uint32_t)idt_decriptors;

    for(int i=0;i< TOTAL_INTERRUPTS;i++)
    {
        idt_set(i,no_interrupt);
    }
    idt_set(0, idt_zero);
    idt_set(0x21,int21h);
    idt_set(0x80, isr80h_wrapper);

    //load the interrupt descriptor table.
    idt_load(&idtr);
}

void isr80h_register_command(int command_id, ISR80H_COMMAND command)
{
    if(command_id <=0|| command_id >=TMOS_MAX_ISR80H_COMMANDS)
    {
        panic("the command is out of bound\n");
    }

    if(isr80h_commands[command_id])
    {
        panic("attempting to overwrite an existing command.\n");
    }

    isr80h_commands[command_id] =command;
}

void* isr80h_handler_command(int command, struct interrupt_frame* frame)
{
    void* result=0;

    if(command <=0 || command >=TMOS_MAX_ISR80H_COMMANDS)
    {
        //invalid commands.
        return 0;
    }

    ISR80H_COMMAND command_func =isr80h_commands[command];
    if(!command_func)
    {
        return 0;
    }

    result =command_func(frame);
    return result;
}

void* isr80h_handler(int command, struct interrupt_frame* frame)
{
    void* res=0;
    kernel_page();
    task_current_save_state(frame);//for multitasking purposes.
    res =isr80h_handler_command(command, frame);
    task_page();
    return res;
}

