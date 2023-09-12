#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"


struct idt_descriptor idt_decriptors[TOTAL_INTERRUPTS];
struct idtr_ idtr;

extern void idt_load(struct idtr_* ptr);

void idt_zero()
{
    print("Oops!!, Divide by zero error.\n");
}
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

    idt_set(0, idt_zero);
    
    //load the interrupt descriptor table.
    idt_load(&idtr);
}


