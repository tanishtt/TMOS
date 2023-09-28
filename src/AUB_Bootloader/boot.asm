ORG 0x7c00
BITS 16

; cli
; mov ax, 0x07C0
; mov ds, ax
; BPB (Bios Parameter Block) starts here
jmp short skip_bpb_to_start ;3 bytes [7C00+0x00]
nop
    
; OEM Identifier (8 bytes)
OEMIdentifier:     ; The first 8 Bytes (3 - 10) is the version of DOS being used.
    db 'TMOS    '   ; Default value recommended by Microsoft

BytesPerSector:       ; Offset 0x0B;512 sector(value)
    dw 0x200              ;(Little-endian format)

SectorsPerCluster:    ; Offset 0x0D
    db 0x80                ; 128 sector per cluster

ReservedSectors:       ; Offset 0x0E
    dw 200                ; 200 reserved sector (including boot record)
    ;here our kernel will be, we will not keep kernel in file system. in this 200 sectors.

FATCopies:         ; Offset 0x10
    db 0x02                ; 2 File Allocation Tables (FATs)

RootDirEntries:   ; Offset 0x11;will upddate later
    dw 0x40              ; 64 entries for root directory (FAT16)

NumSectors:          ; Offset 0x13; 0 total sectors (will be updated later)
    dw 0                ; Large sector count is not used

MediaType:        ; Offset 0x15
    db 0xF8             ; Media descriptor type (standard harddisk)

SectorsPerFat:        ; Offset 0x16;will update later
    dw 0x100                ; 256 sectors per FAT (FAT16)

SectorsPerTrack:      ; Offset 0x18
    dw 0x20               ; 18 sectors per track

NumberOfHeads:        ; Offset 0x1A
    dw 0x40                ; 2 heads (sides)

HiddenSectors:         ; Offset 0x1C
    dd 0                ; 0 hidden sectors (LBA of partition start)

; Large sector count for volumes with more than 65535 sectors
SectorsBig:     ; Offset 0x20
    dd 0x773594                ; Initialize to 7812500 (will be updated if needed)
   
;BPB ends here.

;extended BPB
DriveNumber         db 0x80 ;harddisk
WinNTBit            db 0x00
Signature           db 0x29
VolumeID            dd 0xD105
VolumeIDString      db 'TMOS BOOT  '
SystemIDString      db 'FAT16   '





skip_bpb_to_start:
    sti
    jmp 0x00:step2  ;CS will be updated here.
                    ;CS references memory starting from 0x00000 in the 1 MB real mode address space
;n this real mode state, CS, DS, ES, SS, and the stack pointer all reference memory starting from 0x00000 in the 1 MB real mode address space.
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
    ;cs will point to gdt base address in 1mb
    ;CODE_SEG is defined as the offset of the code segment descriptor in your GDT.
    ;load32 is a label within the 32-bit code segment (a 32-bit code segment defined in the GDT).
;GDT
%include "src/AUB_Bootloader/gdt.asm"


 [BITS 32]
 load32:
    ;Disk read
    ;parameters for reading from disk
    mov eax, 1          ;LBA of first sector to read
    mov ecx, 100        ;number of sector
    mov edi, 0x0100000  ;points to memory where data will be stored after reading.
    call ata_lba_read
    jmp CODE_SEG:0x0100000;of code segment , offset 0x0100000,which will in total 00100000 in RAM as cs=0 in gdt.

%include "src/AUB_Bootloader/disk.asm"

times 510-($ - $$) db 0
dw 0xAA55
