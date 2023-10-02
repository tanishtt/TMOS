[BITS 32]

section .asm
global _start

_start:

    mov eax, 0;eax= command, here ex command 0
    int 0x80
    jmp $