BITS 32

global _start
global kernel_registers

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

    ;initialization process for remapping the master Programmable Interrupt Controller (PIC)
    ;ICWs are sent to the PICs' command ports (0x20 for master PIC and 0xA0 for slave PIC in x86 architecture) during the initialization process. These control words help set up the PICs to properly handle hardware interrupts and to avoid conflicts in the interrupt handling process.
    ; Remap the master PIC

    ; Set ICW1 (Initialization Control Word 1) for the master PIC
    mov al, 00010001b   ; Load binary value 00010001b into AL
    out 0x20, al        ; Send the value in AL to port 0x20 (master PIC command port)

    ; Set ICW2 (Initialization Control Word 2) for the master PIC
    mov al, 0x20        ; Load the value 0x20 into AL
    out 0x21, al        ; Send the value in AL to port 0x21 (master PIC data port)
    ;This value represents the interrupt vector offset. It's the value at which the master PIC will start delivering interrupts. In this case, it's 0x20, which corresponds to interrupt vector 0x20 (32 in decimal).
    ;32-255

    ; Set ICW3 (Initialization Control Word 3) for the master PIC
    mov al, 00000001b   ; Load binary value 00000001b into AL
    out 0x21, al        ; Send the value in AL to port 0x21 (master PIC data port)

    ; End of remap of the master PIC

    call kernel_main

    jmp $


kernel_registers:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    ret


times 512-($ - $$) db 0
