#include "../inc_pub/filehandler.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int FileHandler_structSetup(source_file_t *file)
{
    if (file == NULL)
    {
        return(-1);
    }
    file->fd = -1;
    file->addr = NULL;
    file->map = NULL;
    file->size = 0;
    file->map_len = 0;
    file->page_offest = 0;
    file->page_size = 0;
}

int FileHandler_mapFree(source_file_t *file)
{
    int ret = 0;

    if (file->addr == NULL)
    {
        return(-2);
    }
    
    ret = munmap(file->addr, file->addr_len);
    file->addr = NULL;
    file->addr_len = 0;
    return(ret);
}


int FileHandler_fileClose(source_file_t *file)
{
    int ret = 0;
    if (file->fd == -1)
    {
        return (-2);
    }
    if (file->addr != NULL)
    {
        FileHandler_mapFree(file);
    }
    ret = close(file->fd);
    file->fd = -1;
    return(ret);
}

int FileHandler_fileOpen(source_file_t *file, char * path)
{
    struct stat sb;

    if (file->fd != -1)
    {
        FileHandler_fileClose(file);
    }
    file->fd = open(path, O_RDONLY);
    if (file->fd == -1)
    {
        return(-1);
    }
    if (fstat(file->fd, &sb) == -1)
    {
        return(-1);
    }    /* To obtain file size */

    file->size = sb.st_size;
    file->page_size = getpagesize();
    file->addr = NULL;
    file->map = NULL;
    file->map_len = 0;

    return(0);
}

int FileHandler_mapGet(source_file_t *file, size_t length, off_t offset)
{   
    if (file->fd == -1)
    {
        return(-3);
    }
    if (offset >= file->size) {
        return(-1);
    }
    if (length == 0)
    {
        return (-4);
    }
    if (file->addr != NULL)
    {
        FileHandler_mapFree(file);
    }
    if (offset + length > file->size) /* Can't display bytes past end of file */
    {
        length = file->size - offset;
    }
    file->page_offest = offset & ~(file->page_size - 1); /* offset for mmap() must be page aligned */
    file->map_len = length;
    file->addr_len = length + (offset - file->page_offest);
    file->addr = mmap(NULL, file->addr_len, PROT_READ, MAP_PRIVATE, file->fd, file->page_offest);
    if (file->addr == MAP_FAILED)
    {
        return (-2);
    }

    file->map = file->addr + (offset - file->page_offest);
    return(0);
}
