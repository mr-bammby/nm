#ifndef _IG_FILEHANDLER_H_
#define _IG_FILEHANDLER_H_

#include <sys/types.h>


typedef struct source_file_s
{
    int fd;
    void* addr; 
    void* map;
    size_t addr_len;
    size_t map_len;
    off_t page_offest;
    size_t size;
    int page_size;
} source_file_t;

int FileHandler_structSetup(source_file_t *file);
int FileHandler_fileOpen(source_file_t *file, const char *path);
int FileHandler_fileClose(source_file_t *file);
int FileHandler_mapGet(source_file_t *file, size_t length, off_t offset);
int FileHandler_mapFree(source_file_t *file);

#endif /* _IG_FILEHANDLER_H_ */