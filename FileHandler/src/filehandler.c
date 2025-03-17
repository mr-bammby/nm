/**
 * @file filehandler.c
 * @brief File handling functions for ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file contains functions for managing file operations in ft_nm,
 * including opening, closing, and memory mapping files for parsing symbol data.
 */

#include "../inc_pub/filehandler.h"
#include <fcntl.h>      // For open, O_RDONLY
#include <stdlib.h>     // For NULL
#include <sys/mman.h>   // For mmap, munmap, MAP_FAILED
#include <sys/stat.h>   // For fstat, struct stat
#include <sys/types.h>  // For off_t
#include <unistd.h>     // For close, getpagesize

/**
 * @brief Macro for failed standard function calls
 */
#define FH_CALL_FAILED -1      // Indicates failure return value from standard functions

/**
 * @brief Initializes a source_file_t structure with default values
 * @param[in,out] file Pointer to the source_file_t structure to initialize
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file is NULL
 */
int FileHandler_structSetup(source_file_t *file)
{
    if (file == NULL)
    {
        return FH_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    file->fd = -1;                 // Initialize file descriptor as invalid
    file->addr = NULL;             // No mapped address yet
    file->map = NULL;              // No mapped data pointer
    file->size = 0;                // File size unknown
    file->map_len = 0;             // No mapped length
    file->page_offset = 0;         // No page offset (fixed typo)
    file->page_size = 0;           // Page size unset
    return FH_SUCCESS;             // Success
}

/**
 * @brief Frees the memory mapping associated with a file
 * @param[in,out] file Pointer to the source_file_t structure
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file is NULL,
 *             FH_ERR_NO_MAPPING if no mapping exists, or munmap error code on failure
 */
int FileHandler_mapFree(source_file_t *file)
{
    int ret = FH_SUCCESS;

    if (file == NULL)
    {
        return FH_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    if (file->addr == NULL)
    {
        return FH_ERR_NO_MAPPING;  // No mapping to free
    }
    
    ret = munmap(file->addr, file->addr_len);  // Unmap the memory region
    file->addr = NULL;                         // Clear address
    file->addr_len = 0;                        // Reset mapped length
    return ret;                                // Return munmap result
}

/**
 * @brief Closes an open file and frees associated resources
 * @param[in,out] file Pointer to the source_file_t structure
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file is NULL,
 *             FH_ERR_NOT_OPEN if file is not open, or close error code on failure
 */
int FileHandler_fileClose(source_file_t *file)
{
    int ret = FH_SUCCESS;

    if (file == NULL)
    {
        return FH_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    if (file->fd == -1)
    {
        return FH_ERR_NOT_OPEN;    // File not open
    }
    if (file->addr != NULL)
    {
        FileHandler_mapFree(file);  // Free any existing mapping
    }
    ret = close(file->fd);          // Close the file descriptor
    file->fd = -1;                  // Mark as closed
    return ret;                     // Return close result
}

/**
 * @brief Opens a file and initializes its source_file_t structure
 * @param[in,out] file Pointer to the source_file_t structure
 * @param[in] path Path to the file to open
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file or path is NULL,
 *             FH_ERR_INTERNAL on failure to open or stat, or close error code on cleanup failure
 */
int FileHandler_fileOpen(source_file_t *file, const char *path)
{
    struct stat sb;

    if (file == NULL || path == NULL)
    {
        return FH_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    if (file->fd != -1)
    {
        FileHandler_fileClose(file);  // Close any existing open file
    }
    file->fd = open(path, O_RDONLY);  // Open file in read-only mode
    if (file->fd == -1)
    {
        return FH_ERR_INTERNAL;       // Failed to open file
    }
    if (fstat(file->fd, &sb) == FH_CALL_FAILED)
    {
        int ret = FileHandler_fileClose(file);  // Close file on stat failure
        return ret ? ret : FH_ERR_INTERNAL;     // Return close error or internal failure
    }

    file->size = sb.st_size;          // Set file size
    file->page_size = getpagesize();  // Get system page size
    file->addr = NULL;                // No mapped address yet
    file->map = NULL;                 // No mapped data pointer
    file->map_len = 0;                // No mapped length

    return FH_SUCCESS;                // Success
}

/**
 * @brief Maps a portion of the file into memory
 * @param[in,out] file Pointer to the source_file_t structure
 * @param[in] length Length of the data to map
 * @param[in] offset Offset within the file to start mapping
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file is NULL or offset exceeds file size,
 *             FH_ERR_NOT_OPEN if file is not open, FH_ERR_ZERO_LENGTH if length is zero,
 *             FH_ERR_NO_MAPPING if mmap fails
 */
int FileHandler_mapGet(source_file_t *file, size_t length, off_t offset)
{   
    if (file == NULL)
    {
        return FH_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    if (file->fd == FH_CALL_FAILED)
    {
        return FH_ERR_NOT_OPEN;    // File not open
    }
    if ((size_t)offset >= file->size)
    {
        return FH_ERR_NULL_INPUT;  // Offset beyond file size
    }
    if (length == 0)
    {
        return FH_ERR_ZERO_LENGTH;  // Invalid length
    }
    if (file->addr != NULL)
    {
        FileHandler_mapFree(file);  // Free existing mapping
    }
    if (offset + length > file->size)  // Truncate to file size if beyond end
    {
        length = file->size - offset;
    }
    file->page_offset = offset & ~(file->page_size - 1);  // Align offset to page boundary
    file->map_len = length;                               // Set mapped data length
    file->addr_len = length + (offset - file->page_offset);  // Total mapped region length
    file->addr = mmap(NULL, file->addr_len, PROT_READ, MAP_PRIVATE, file->fd, file->page_offset);  // Map file
    if (file->addr == MAP_FAILED)
    {
        return FH_ERR_NO_MAPPING;  // Mapping failed
    }

    file->map = file->addr + (offset - file->page_offset);  // Set pointer to requested offset
    return FH_SUCCESS;                                      // Success
}
