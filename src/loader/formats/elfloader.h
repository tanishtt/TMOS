#ifndef ELFLOADER_H
#define ELFLOADER_H

#include "elf.h"
#include "config.h"
#include<stdint.h>
#include<stddef.h>

struct elf_file
{
    char filename[MAX_PATH];

    int in_memory_size;

    
    void* elf_memory;//physical memory address that this elf file is loaded at.
  
    void* virtual_base_address;//virtual base address of this binary

    void* virtual_end_address;//ending virtual address of this binary.


    void* physical_base_address;//physical base address of this binary.
    
    void* physical_end_address;//physical end address of this binary.



};

int elf_load(const char* filename, struct elf_file** file_out);
void elf_close(struct elf_file* file);

void* elf_virtual_base(struct elf_file* file);
void* elf_virtual_end(struct elf_file* file);
void* elf_phys_base(struct elf_file* file);
void* elf_phys_end(struct elf_file* file);

struct elf_header* elf_header(struct elf_file* file);
struct elf32_shdr* elf_sheader(struct elf_header* header);
struct elf32_phdr* elf_pheader(struct elf_header* header);
struct elf32_phdr* elf_program_header(struct elf_header* header, int index);
void* elf_memory(struct elf_file* file);
struct elf32_shdr* elf_section(struct elf_header* header, int index);

void* elf_phdr_phys_addrss(struct elf_file*file , struct elf32_phdr* phdr);

#endif
