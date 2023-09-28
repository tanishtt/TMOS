#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "status.h"
#include "memory/heap/kheap.h"
#include "string/string.h"
#include "kernel.h"
#include "fat/fat16.h"



struct filesystem* filesystems[MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[MAX_FILE_DESCRIPTORS];

//inserting file systems in the array.
static struct filesystem** fs_get_free_filesystem()
{
    //we can accomodate max 12 filesystems, if space is there, the we can insert filesystem. 
    int i=0;
    for(i=0; i< MAX_FILESYSTEMS ;i++)
    {
        if(filesystems[i] ==0)
        {
            return &filesystems[i];
        }
    }
    return 0;
}

//ex- insert ntfs in file system array using this array. 
void fs_insert_filesystem(struct filesystem* filesystem)
{
    struct filesystem** fs;
    fs =fs_get_free_filesystem();
    if(!fs)
    {
        print("not able to insert filesystem");
        while(1){}
    }
    *fs = filesystem;
}

static void fs_static_load()
{
    //here you can any file system you want, here we uploading fat16.
    fs_insert_filesystem(fat16_init());
}

void fs_load()
{
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

void fs_init()
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static int file_new_descriptor( struct file_descriptor** desc_out)
{
    int res= -ENOMEM;
    for(int i=0; i< MAX_FILE_DESCRIPTORS; i++)
    {
        if(file_descriptors[i] ==0)
        {
            struct file_descriptor* desc= kzalloc(sizeof( struct file_descriptor));
            desc->index =i+1;//as decriptor starts from one.
            file_descriptors[i] =desc;
            *desc_out =desc;
            res=0;
            break;
        }
    }
    return res;
}

static struct file_descriptor* file_get_descriptor(int fd)
{
    if(fd <=0 || fd>= MAX_FILE_DESCRIPTORS)
    {
        return 0;
    }
    int index= fd-1;//as descriptor starts with 1
    return file_descriptors[index];
}

struct filesystem* fs_resolve(struct disk* disk)
{
    struct filesystem* fs=0;
    for(int i=0;i< MAX_FILESYSTEMS; i++)
    {
        if(filesystems[i] !=0 && filesystems[i]->resolve(disk) ==0)//means it can manage the disk, as it has file system type of his.
        {
            fs= filesystems[i];
            break;
        }
    }
    return fs;
}

int fopen(const char* filename, const char* mode)
{
    return -EIO;
}