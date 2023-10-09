[BITS 32]

section .asm

global print:function
global sub_getkey:function
global sub_malloc:function
global sub_free:function
global sub_putchar:function
global sub_process_load_start:function


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
sub_getkey:
    push ebp
    mov ebp,esp
    mov eax, 2;command getkey
    int 0x80
    pop ebp
    ret


;void* main_malloc(size_t size);
sub_malloc:
    push ebp
    mov ebp, esp
    mov eax, 4;4=>malloc, allocate memory for process.
    push dword[ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret

sub_free:
    push ebp
    mov ebp, esp
    mov eax, 5 ;command 5 free(Frees the allocated memory for process)
    push dword[ebp+8] ;variable ptr
    int 0x80
    add esp, 4
    pop ebp
    ret

; void sub_putchar(char c)
sub_putchar:
    push ebp
    mov ebp, esp
    mov eax, 3 ;command putchar
    push dword [ebp+8] ;variable c
    int 0x80
    add esp, 4
    pop ebp
    ret

;void sub_process_load_start(const char* filename);
sub_process_load_start:
    push ebp
    mov ebp, esp
    mov eax, 6;process load start, start a process
    push dword[ebp+8];filename parameter.
    int 0x80
    add esp, 4
    pop ebp
    ret


