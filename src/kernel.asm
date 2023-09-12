BITS 32

global _start
global problem_

extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable the A20 line;The A20 line (Address line 20) is used to control memory addressing in protected mode and is separate from the regular address bus. When the A20 line is enabled, it allows the CPU to access memory beyond the 1 MB limit. When it's disabled, memory access is limited to the first 1 MB.
    in al, 0x92
    or al, 2
    out 0x92, al

    call kernel_main

    jmp $

problem_:
    mov eax, 0
    div eax
times 512-($ - $$) db 0
