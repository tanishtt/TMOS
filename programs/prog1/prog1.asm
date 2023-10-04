[BITS 32]

section .asm
global _start

_start:
    ; push 30
    ; push 20
    ; mov eax, 0;eax= command, here ex command 0
    ; int 0x80
    ; add esp, 8


_keygetputloop:
    call getkey
    push eax
    mov eax, 3;putting char from terminal
    int 0x80
    add esp, 4

    jmp _keygetputloop

getkey:
    mov eax, 2;command to getkey.
    int 0x80
    cmp eax, 0x00
    je getkey
    ret

section .data
message: db 'hello ji',0