
#ifndef IDT_H
#define IDT_H
#include <stdint.h>

struct idt_descriptor
{
    uint16_t offset_1; //offset bits 0-15
    uint16_t selector; //code segment selector in our gdt or ldt (0-15)
    uint8_t zero; //unused bit
    uint8_t type_attributes;// gate type, dpl, and p fields
    uint16_t offset_2; //offset bits 16-31
    
}__attribute__((packed));

struct idtr_
{
    uint16_t limit; //size of descriptor table(1byte or 4kb)
    uint32_t base;//base address of the start of the idt in memory
}__attribute__((packed));


void idt_init();
void enable_interrupts();
void disable_interrupts();

#endif