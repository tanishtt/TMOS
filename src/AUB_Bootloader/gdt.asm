; GDT (Global Descriptor Table) definition
gdt_start:
    ; Null descriptor
    gdt_null:
        dd 0x00000000 ; Limit (bits 0-15)
        dd 0x00000000 ; Base (bits 0-31)

    ; Code segment descriptor
    gdt_code:
        dw 0xFFFF     ; Limit (bits 0-15)
        dw 0x0000     ; Base (bits 0-15)
        db 0x00       ; Base (bits 16-23)
        db 10011010b       ; Flags: Present, Privilege Level 0, Code Segment, Readable
        db 11001111b       ; Flags: Granularity, 32-bit Default Operation
        db 0x00       ; Base (bits 24-31)

    ; Data segment descriptor
    gdt_data:
        dw 0xFFFF     ; Limit (bits 0-15)
        dw 0x0000     ; Base (bits 0-15)
        db 0x00       ; Base (bits 16-23)
        db 10010010b       ; Flags: Present, Privilege Level 0, Data Segment, Writable
        db 11001111b       ; Flags: Granularity, 32-bit Default Operation
        db 0x00       ; Base (bits 24-31)

gdt_end: ; End of GDT

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of GDT - 1
    dd gdt_start ; Start address(base address) of GDT

; Define segment selector offsets for convenience
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
