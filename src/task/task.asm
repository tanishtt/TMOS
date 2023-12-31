[BITS 32]
 section .asm


global restore_general_purpose_registers
global task_return
global user_registers
;in this task register, we are storing state of current state in stack 
;and converting to new task, whose values are in registers, by adding values from registers we will go to new task

;void task_return(struct registers* regs);
task_return:
    mov ebp, esp
    ;PUSH THE DATA SEGMENT(SS)
    ;PUSH THE STACK ADDRESS
    ;PUSH THE FLAGS
    ;PUSH THE CODE SEGMENT
    ;PUSH IP

    ;access the structure passed to us
    mov ebx, [ebp+4]
    ;push data/stack selector
    push dword [ebx+44]
    ;push stack pointer
    push dword [ebx+40]

    ;push flags
    pushf
    pop eax
    or eax, 0x200
    push eax

    ;push code segment
    push dword [ebx+32]

    ;push ip to execute
    push dword [ebx+28]

    ;setup the segment registers
    mov ax, [ebx+44]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push dword [ebp+4]
    call restore_general_purpose_registers
    add esp, 4

    ;leave kernel land and execute in user land.
    iretd
    
;void restore_general_purpose_registers(struct registers* regs);
restore_general_purpose_registers:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]
    mov edi, [ebx]
    mov esi, [ebx+4]
    mov ebp, [ebx+8]
    mov edx, [ebx+16]
    mov ecx, [ebx+20]
    mov eax, [ebx+24]
    mov ebx, [ebx+12]
    pop ebp
    ret

;void user_registers()
user_registers:
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret