section .asm

extern int21h_handler
extern no_interrupt_handler
extern isr80h_handler

global int21h
global no_interrupt 
global idt_load
global enable_interrupts
global disable_interrupts
global isr80h_wrapper


enable_interrupts:
    sti
    ret
disable_interrupts:
    cli
    ret

idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]
    lidt [ebx]

    pop ebp
    ret

int21h:
    pushad
    call int21h_handler
    popad
    iret

no_interrupt:
    pushad
    call no_interrupt_handler
    popad
    iret

isr80h_wrapper:
    cli
    ;INTERRUPT FRAME START
    ;already pushed to us, by the processor upon entry to this interrupt.
    ;uint32_t ip
    ;uint32_t cs;
    ;uint32_t flags
    ;uint32_t sp;
    ;uint32_t ss;
    ;pushes general purpose registers to the stack.

    ;pushes all general purpose registers.
    pushad
    ;INTERRUPT FRAME END

    ;push the stack pointer so that we are pointing to interrupt frame.
    push esp

    ;eax holds our command, it contains the command number.
    ;push it to the stack for isr80h_handler.
    push eax
    call isr80h_handler
    mov dword[tmp_res], eax
    add esp, 8

    ;restore general purpose registers.
    popad

    mov eax, [tmp_res]
    iretd

section .data
;inside here stored , return result from isr80h_handler.
tmp_res: dd 0
