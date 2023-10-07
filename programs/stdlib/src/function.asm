[BITS 32]

global print:function


;void print(const char* message)
print:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 1;coomand print
    int 0x80
    add esp, 4;stack will be restored after 4 bytes, as 4 bytes will be message.
    pop ebp
    ret
