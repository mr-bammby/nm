/**
 * @file filehandler.h
 * @brief Header file for file handling operations in ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header declares structures and functions for managing file operations
 * in ft_nm, including opening, closing, and memory mapping files for parsing
 * symbol data.
 */

#ifndef _IG_FILEHANDLER_H_
#define _IG_FILEHANDLER_H_

#include <sys/types.h>  // For off_t, size_t

/**
 * @brief Error codes for file handling operations
 */
enum FileHandler_Error {
    FH_SUCCESS = 0,            /**< Success */
    FH_ERR_NULL_INPUT = -1,    /**< Invalid input (NULL pointer) */
    FH_ERR_NO_MAPPING = -2,    /**< No mapping exists */
    FH_ERR_NOT_OPEN = -3,      /**< File is not open */
    FH_ERR_ZERO_LENGTH = -4,   /**< Requested mapping length is zero */
    FH_ERR_INTERNAL = -5       /**< Internal failure (e.g., system call error) */
};

/**
 * @brief Structure representing a source file for memory mapping
 */
typedef struct source_file_s
{
    int fd;                /**< File descriptor */
    void* addr;            /**< Starting address of the memory-mapped region */
    void* map;             /**< Pointer to the requested offset within the mapped region */
    size_t addr_len;       /**< Length of the entire mapped region */
    size_t map_len;        /**< Length of the requested data within the mapped region */
    off_t page_offset;     /**< Page-aligned offset used for mapping */
    size_t size;           /**< Total size of the file */
    int page_size;         /**< System page size */
} source_file_t;

/**
 * @brief Initializes a source_file_t structure with default values
 * @param[in,out] file Pointer to the source_file_t structure to initialize
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file is NULL
 */
int FileHandler_structSetup(source_file_t *file);

/**
 * @brief Opens a file and initializes its source_file_t structure
 * @param[in,out] file Pointer to the source_file_t structure
 * @param[in] path Path to the file to open
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file or path is NULL,
 *             FH_ERR_INTERNAL on failure to open or stat, or close error code on cleanup failure
 */
int FileHandler_fileOpen(source_file_t *file, const char *path);

/**
 * @brief Closes an open file and frees associated resources
 * @param[in,out] file Pointer to the source_file_t structure
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file is NULL,
 *             FH_ERR_NOT_OPEN if file is not open, or close error code on failure
 */
int FileHandler_fileClose(source_file_t *file);

/**
 * @brief Maps a portion of the file into memory
 * @param[in,out] file Pointer to the source_file_t structure
 * @param[in] length Length of the data to map
 * @param[in] offset Offset within the file to start mapping
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file is NULL or offset exceeds file size,
 *             FH_ERR_NOT_OPEN if file is not open, FH_ERR_ZERO_LENGTH if length is zero,
 *             FH_ERR_NO_MAPPING if mmap fails
 */
int FileHandler_mapGet(source_file_t *file, size_t length, off_t offset);

/**
 * @brief Frees the memory mapping associated with a file
 * @param[in,out] file Pointer to the source_file_t structure
 * @return int FH_SUCCESS on success, FH_ERR_NULL_INPUT if file is NULL,
 *             FH_ERR_NO_MAPPING if no mapping exists, or munmap error code on failure
 */
int FileHandler_mapFree(source_file_t *file);

#endif /* _IG_FILEHANDLER_H_ */
