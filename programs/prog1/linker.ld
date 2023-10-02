ENTRY(_start)/*kernel's execution begins.*/
OUTPUT_FORMAT(binary)/*output file will be a binary file*/
SECTIONS
{
    . = 0x400000;/*current memory location pointer.
    /*4KB boundary,a common page size*/
    .text : ALIGN(4096)
    {
        *(.text)
    }
    .asm : ALIGN(4096)
    {
        *(.asm)
    }
    .rodata : ALIGN(4096)/*read-only data, such as constant values or string literals.*/
    {
        *(.rodata)
    }

    .data : ALIGN(4096)/* initialized data. initialized global and static variables.*/
    {
        *(.data)
    }

    .bss : ALIGN(4096)/*uninitialized data,(global and local)*/
    {
        *(COMMON)/*uninitialized global variables with common storage.ex-int commonVar in abc.c;extern int commonVar in pqr.c;*/
        *(.bss)
    }

    
    /*The actual memory addresses of the sections (.text, .data, .bss, etc.) are determined by the loader or linker at runtime, based on the starting address (1M) */
}