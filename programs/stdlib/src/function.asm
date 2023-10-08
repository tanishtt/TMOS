[BITS 32]

global print:function
global getkey:function

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

;int getkey()
getkey:
    push ebp
    mov ebp,esp
    mov eax, 2;command getkey
    int 0x80
    pop ebp
    ret

