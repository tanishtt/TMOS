#ifndef ELF_H
#define ELF_H


#include <stdint.h>
#include <stddef.h>

//read, write, executable flags.
#define PF_X 0x01
#define PF_W 0x02
#define PF_R 0x04

//program header values.
#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6

//section header values.
#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_LOPROC 12
#define SHT_HIPROC 13
#define SHT_LOUSER 14
#define SHT_HIUSER 15

//Identifies the object file type
#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4

//magic number ->16
#define EI_NIDENT 16
#define EI_CLASS 4
#define EI_DATA 5

//class 32 or 64
#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2

//lsb msb
#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define SHN_UNDEF 0

typedef uint16_t elf32_half;
typedef uint32_t elf32_word;
typedef int32_t elf32_sword;//signed 4b
typedef uint32_t elf32_addr;//unsigned 4b
typedef int32_t elf32_off;

//program header.
struct elf32_phdr{
    elf32_word p_type;     // Segment type (e.g., LOAD, DYNAMIC, etc.)
    elf32_off  p_offset;   // Offset of the segment in the file
    elf32_addr p_vaddr;    // Virtual address where the segment should be loaded
    elf32_addr p_paddr;    // Physical address (not used on most systems)
    elf32_word p_filesz;   // Size of the segment in the file
    elf32_word p_memsz;    // Size of the segment in memory (may be larger)
    elf32_word p_flags;    // Segment flags (e.g., read, write, execute)
    elf32_word p_align;    // Required alignment of the segment
} __attribute__((packed));

//section header.
struct elf32_shdr{
    elf32_word  sh_name;       // offset to the section name in the section name string table
    elf32_word  sh_type;       // section type (e.g., SHT_PROGBITS, SHT_SYMTAB, etc.)
    elf32_word  sh_flags;      // section attributes (e.g., SHF_WRITE, SHF_ALLOC, etc.)
    elf32_addr  sh_addr;       // virtual address where the section should be loaded
    elf32_off   sh_offset;     // offset in the file where the section's data begins
    elf32_word  sh_size;       // size of the section in bytes
    elf32_word  sh_link;       // index of a related section (e.g., symbol table)
    elf32_word  sh_info;       // additional section-specific information
    elf32_word  sh_addralign;  // required alignment of the section
    elf32_word  sh_entsize;    // size of each entry in the section (if applicable)
} __attribute__((packed));

//elf header.
struct elf_header
{
    unsigned char e_ident[EI_NIDENT];
    elf32_half e_type;
    elf32_half e_machine;      // architecture (e.g., x86, ARM, etc.)
    elf32_word e_version;      // version of the ELF format
    elf32_addr e_entry;        // entry point address (where program execution begins)
    elf32_off e_phoff;         // offset to the program header table
    elf32_off e_shoff;         // offset to the section header table
    elf32_word e_flags;        // processor-specific flags
    elf32_half e_ehsize;       // size of this ELF header (bytes)
    elf32_half e_phentsize;    // size of each entry in the program header table
    elf32_half e_phnum;        // number of entries in the program header table
    elf32_half e_shentsize;    // size of each entry in the section header table
    elf32_half e_shnum;        // number of entries in the section header table
    elf32_half e_shstrndx;     // index of the section header table entry containing section names
}__attribute__((packed));

struct elf32_dyn{
    elf32_sword d_tag;      // entry type (e.g., DT_HASH, DT_STRTAB, etc.)
    union {
        elf32_word d_val;   // integer value associated with the entry
        elf32_addr d_ptr;   // address value associated with the entry
    } d_un;
}__attribute__((packed));//hold dynamic entry information in an ELF context, where the d_tag field specifies the type of dynamic entry, and the d_un union can hold either an integer value or an address value depending on the entry type.


struct elf32_sym {
    elf32_word st_name;    // Symbol name's index in the string table. It specifies the name of the symbol represented by this entry.
    elf32_addr st_value;   // Symbol's value or address
    elf32_word st_size;    // Size of the symbol (in bytes)
    unsigned char st_info; // Symbol's type and binding attributes
    unsigned char st_other; // Reserved
    elf32_half st_shndx;   // Section index where the symbol is defined or undefined
} __attribute__((packed));
//This elf32_sym structure is essential for the linking process, allowing the linker to resolve symbols and establish connections between different parts of an ELF file, such as functions and variables defined in different sections.

void* elf_get_entry_ptr(struct elf_header* elf_header);
uint32_t elf_get_entry(struct elf_header* elf_header);

#endif
