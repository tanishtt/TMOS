#include "process.h"
#include "config.h"
#include "status.h"
#include "task/task.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "fs/file.h"
#include "string/string.h"
#include "kernel.h"
#include "memory/paging/paging.h"
#include "loader/formats/elfloader.h"



// current process that is running.
struct process *current_process = 0;

static struct process *processes[MAX_PROCESSES] = {};

static void process_init(struct process *process)
{
    memset(process, 0, sizeof(struct process));
}

struct process *process_current()
{
    return current_process;
}

struct process *process_get(int process_id)
{
    if (process_id < 0 || process_id >= MAX_PROCESSES)
    {
        return NULL;
    }

    return processes[process_id];
}

static int process_find_free_allocation_index(struct process* process)
{
    int res=-ENOMEM;
    for(int i=0;i<MAX_PROGRAM_ALLOCATIONS; i++)
    {
        if(process->allocation[i].ptr ==0)
        {
            res =i;
            break;
        }
    }
    return res;
}

void* process_malloc(struct process* process, size_t size)
{
    void* ptr= kzalloc(size);
    if(!ptr)
    {
        goto out_err;
    }
    int index= process_find_free_allocation_index(process);
    if(index<0)
    {
        goto out_err;
    }

    int res= paging_map_to(process->task->page_directory,ptr ,ptr,paging_align_address(ptr+size), PAGING_IS_WRITABLE| PAGING_IS_PRESENT |PAGING_ACCESS_FROM_ALL);

    if(res< 0)
    {
        goto out_err;
    }
    process->allocation[index].ptr =ptr;
    process->allocation[index].size =size;

    return ptr;

out_err:
if(ptr)
{
    kfree(ptr);
}
    return 0;
}

static bool process_is_process_pointer(struct process* process, void* ptr)
{
    for(int i=0; i<MAX_PROGRAM_ALLOCATIONS; i++)
    {
        if(process->allocation[i].ptr ==ptr)
        {
            return true;
        }
    }
    return false;
}

static void process_allocation_unjoin(struct process* process, void* ptr)
{
    for (int i =0; i <MAX_PROGRAM_ALLOCATIONS; i++)
    {
        if (process->allocation[i].ptr ==ptr)
        {
            process->allocation[i].ptr =0x00;
            process->allocation[i].size =0;

        }
    }
}

static struct process_allocation* process_get_allocation_by_addr(struct process* process, void* addr)
{
    for (int i = 0; i < MAX_PROGRAM_ALLOCATIONS; i++)
    {
        if (process->allocation[i].ptr ==addr)
            {return &process->allocation[i];}
    }

    return 0;
}

void process_free(struct process* process, void* ptr)
{
    // if(!process_is_process_pointer(process, ptr))
    // {//not our pointer, so can't free.
    //     return;
    // }

    //unlink the pages from the process for the given address.
    struct process_allocation* allocation =process_get_allocation_by_addr(process, ptr);

    if(!allocation)
    {
        return;
    }

    int res =paging_map_to(process->task->page_directory, allocation->ptr, allocation->ptr,paging_align_address(allocation->ptr +allocation->size), 0x00);
    //prevent process to access the freed memory.
    if(res<0)
    {
        return;
    } 
    //unjoin the allocation
    process_allocation_unjoin(process, ptr);

    //we can now free the memory.
    kfree(ptr);

}

int process_switch(struct process* process)
{
    //all process print to same terminal, like that.
    current_process =process;
    return 0;
}

static int process_load_binary(const char *filename, struct process *process)
{   //print("process_load_binary\n");
    int res = 0;
    int fd = fopen(filename, "r");
    if (!fd)
    {//print(filename);
    //print("\n");
        res = -EIO;
        goto out;
    }

    struct file_stat stat;
    res = fstat(fd, &stat);
    if (res != ALL_OK)
    {
        goto out;
    }

    void *program_data_ptr = kzalloc(stat.filesize);
    if (!program_data_ptr)
    {
        res = -ENOMEM;
        goto out;
    }

    if (fread(program_data_ptr, stat.filesize, 1, fd) != 1)
    {
        res = -EIO;
        goto out;
    }
    process->filetype =PROCESS_FILETYPE_BINARY;
    process->ptr = program_data_ptr;
    process->size = stat.filesize;

out:
    fclose(fd);
    return res;
}

static int process_load_elf(const char* filename, struct process* process)
{
    int res=0;
    struct elf_file* elf_file =0;
    res= elf_load(filename, &elf_file);
    if(ISERR(res))
    {
        goto out;
    }

    process->filetype =PROCESS_FILETYPE_ELF;
    process->elf_file =elf_file;


out:    
    return res;
}

static int process_load_data(const char *filename, struct process *process)
{
    int res = 0;
    res =process_load_elf(filename, process);
    if(res ==-EINFORMAT)
    {//IF NOT IN FORMAT, MEANS IN BIN
        res = process_load_binary(filename, process);
    }
    return res;
}

int process_map_binary(struct process *process)
{
    int res = 0;
    paging_map_to(process->task->page_directory, (void *)TMOS_PROGRAM_VIRTUAL_ADDRESS, process->ptr, paging_align_address(process->ptr + process->size), PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITABLE);
    return res;
}

static int process_map_elf(struct process* process)
{
    int res=0;
    struct elf_file* elf_file= process->elf_file;
    struct elf_header* header =elf_header(elf_file);
    struct elf32_phdr* phdrs =elf_pheader(header);

    for(int i=0; i<header->e_phnum;i++)
    {
        struct elf32_phdr* phdr= &phdrs[i];
        void* phdr_phys_address= elf_phdr_phys_addrss(elf_file, phdr);
        int flags =PAGING_IS_PRESENT |PAGING_ACCESS_FROM_ALL;

        if(phdr->p_flags &PF_W)
        {
           flags|= PAGING_IS_WRITABLE;
        }
        res=paging_map_to(process->task->page_directory, paging_align_to_lower_page((void*)phdr->p_vaddr), paging_align_to_lower_page(phdr_phys_address), paging_align_address(phdr_phys_address+ phdr->p_memsz),flags);//, PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITABLE);
        if(ISERR(res))
        {
            break;
        }
    }

    return res;
}

int process_map_memory(struct process *process)
{
    int res = 0;

    switch(process->filetype)
    {
        case PROCESS_FILETYPE_ELF:
            res =process_map_elf(process);
        break;

        case PROCESS_FILETYPE_BINARY:
            res =process_map_binary(process);
        break;

        default:
            panic("process_map_memory: Invalid filetype\n");
    }

    //res = process_map_binary(process);
    if(res<0)
    {
        goto out;
    }
    paging_map_to(process->task->page_directory, (void*)TMOS_PROGRAM_VIRTUAL_STACK_ADDRESS_END, process->stack, paging_align_address(process->stack+ TMOS_USER_PROGRAM_STACK_SIZE), PAGING_IS_PRESENT| PAGING_ACCESS_FROM_ALL| PAGING_IS_WRITABLE);

out:

    return res;
}

int process_get_free_slot()
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (processes[i] == 0)
        {
            return i;
        }
    }
    return -EINSTKN;
}

int process_load(const char *filename, struct process **process)
{
    int res = 0;
    int process_slot = process_get_free_slot();
    if (process_slot < 0)
    {
        res = -ENOMEM;
        goto out;
    }
    res = process_load_for_slot(filename, process, process_slot);
out:
    return res;
}

int process_load_switch(const char* filename, struct process** process)
{
    int res= process_load(filename, process);
    if(res==0)
    {
        process_switch(*process);
    }

    return res;
}

int process_load_for_slot(const char *filename, struct process **process, int process_slot)
{
    int res = 0;
    struct task *task = 0;
    struct process *_process;
    void *program_stack_ptr = 0;

    if (process_get(process_slot) != 0)
    {
        res = -EINSTKN;
        goto out;
    }

    _process = kzalloc(sizeof(struct process));
    if (!_process)
    {
        res = -ENOMEM;
        goto out;
    }

    process_init(_process);
    res = process_load_data(filename, _process);
    if (res < 0)
    {//print("gggg");
        goto out;
    }

    program_stack_ptr = kzalloc(TMOS_USER_PROGRAM_STACK_SIZE);
    if (!program_stack_ptr)
    {
        res = -ENOMEM;
        goto out;
    }

    strncpy(_process->filename, filename, sizeof(_process->filename));
    _process->stack = program_stack_ptr;
    _process->id = process_slot;

    // create a task.
    task = task_new(_process);
    if (ERROR_I(task) == 0)
    {
        res = ERROR_I(task);
        goto out;
    }

    _process->task = task;

    res = process_map_memory(_process);
    if (res < 0)
    {
        goto out;
    }

    *process = _process;

    // add the process to array.
    processes[process_slot] = _process;

out:
    if (ISERR(res))
    {
        if (_process && _process->task)
        {
            task_free(_process->task);
        }
    }
    return res;
}
