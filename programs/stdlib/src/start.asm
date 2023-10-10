[BITS 32]

global _start
extern c_start

section .asm

_start:
    push 50
    push 20
    call c_start
    ret