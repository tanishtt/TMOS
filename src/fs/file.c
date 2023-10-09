#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "status.h"
#include "memory/heap/kheap.h"
#include "string/string.h"
#include "kernel.h"
#include "fat/fat16.h"
#include "disk/disk.h"



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

static void file_free_descriptor(struct file_descriptor* desc)
{
    file_descriptors[desc->index-1]= 0x00;
    kfree(desc);
    
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



FILE_MODE file_get_mode_by_string(const char* str)
{
    FILE_MODE mode =FILE_MODE_INVALID;
    if(strncmp(str, "r", 1)==0)
    {
        mode=FILE_MODE_READ;
    }
    else if(strncmp(str,"w",1)==0)
    {
        mode=FILE_MODE_WRITE;
    }
    else if(strncmp(str, "a",1)==0)
    {
        mode= FILE_MODE_APPEND;
    }
    return mode;
}

int fopen(const char* filename, const char* mode_str)
{//print(filename);
print("\n");
    int res=0;
    struct path_root* root_path =pathparser_parse(filename, NULL);
    if(!root_path)
    {print("No such command exists 1\n");
        res=-EINVARG;
        goto out;
    }
    //we cannot have just a root path 0:/ 
    if(!root_path->first)
    {print("No such command exists 2\n");
        res=-EINVARG;
        goto out;
    }
    //ensure ,the disk we are reading, exists or not.
    struct disk* disk= disk_get(root_path->drive_no);
    if(!disk)
    {print("No such command exists 3\n");
        res=-EIO;
        goto out;

    }
    if(!disk->filesystem)
    {print("No such command exists 4\n");
        res=-EIO;
        goto out;
    }

    FILE_MODE mode =file_get_mode_by_string(mode_str);
    if(mode ==FILE_MODE_INVALID)
    {print("No such command exists 5\n");
        res =-EINVARG;
        goto out;
    }

    void* descriptor_private_data = disk->filesystem->open(disk, root_path->first, mode);
    if(ISERR(descriptor_private_data))
    {print("No such command exists 6\n");
        res= ERROR_I(descriptor_private_data);
        goto out;
    }

    struct file_descriptor* desc =0;
    res= file_new_descriptor(&desc);
    if(res<0)
    {print("No such command exists 7\n");
        goto out;
    }
    desc->filesystem= disk->filesystem;
    desc->private= descriptor_private_data;
    desc->disk=  disk;
    res= desc->index;


out:
    if(res<0)
    {//fopen should not return negative values.
        res=0;
    }
    return res;
}


////////////

int fstat(int fd, struct file_stat* stat)
{
    int res=0;
    struct file_descriptor* desc =file_get_descriptor(fd);
    if(!desc)
    {
        res=-EIO;
        goto out;
    }

    res= desc->filesystem->stat(desc->disk, desc->private, stat);
out:
    return res;
}

int fclose(int fd)
{
    int res=0;
    struct file_descriptor* desc= file_get_descriptor(fd);
    if(!desc)
    {
        res =-EIO;
        goto out;
    }
    res= desc->filesystem->close(desc->private);

    if(res ==ALL_OK)
    {
        file_free_descriptor(desc);
    }
out:
    return res;
}

int fseek(int fd, int offset, FILE_SEEK_MODE whence)
{
    //whence= from what place.
    //fd= file descriptor.

    int res=0;
    struct file_descriptor* desc =file_get_descriptor(fd);

    if(!desc)
    {
        res=-EIO;
        goto out;
    }
    res =desc->filesystem->seek(desc->private, offset, whence);
out:
    return res;
}

int fread(void* ptr, uint32_t size, uint32_t nmemb, int fd)
{
    int res=0;
    if(size==0 || nmemb ==0 ||fd<1)
    {//file descriptor less than 1.
        res= -EINVARG;
        goto out;
    }
    

    struct file_descriptor* desc =file_get_descriptor(fd);
    if(!desc)
    {
        res=-EINVARG;
        goto out;
    }
    res= desc->filesystem->read(desc->disk, desc->private, size, nmemb,(char*)ptr);


out:
    return res;
}






