ORG 0x7c00
BITS 16

_start:
    jmp short start
    nop

 times 33 db 0
 
start:
    jmp 0:step2

step2:
    cli ; Clear Interrupts
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti ; Enables Interrupts

.switch_protected32:
    cli                 ; Disable interrupts to prevent issues during transition
    lgdt [gdt_descriptor] ; Load the Global Descriptor Table (GDT) which defines PM segments

    mov eax, cr0         ; Load the value of CR0 into EAX register
    or eax, 0x1          ; Set the first bit of CR0 to enable protected mode
    mov cr0, eax         ; Store the modified value back in CR0

    jmp CODE_SEG:load32 ; Far jump to a 32-bit code segment to enter protected mode
    
;GDT
%include "gdt.asm"
%include "disk.asm"

 [BITS 32]
 load32:
    ;Disk read
    ;parameters for reading from disk
    mov eax, 1          ;LBA of first sector to read
    mov ecx, 100        ;number of sector
    mov edi, 0x0100000  ;points to memory where data will be stored after reading.
    call ata_lba_read
    jmp CODE_SEG:0x0100000

times 510-($ - $$) db 0
dw 0xAA55
