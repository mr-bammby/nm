/**
 * @file writer_nameprint.c
 * @brief Symbol name printing function for ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file contains a function for printing symbol names to stdout
 * in the ft_nm writer module.
 */

#include "../inc_priv/writer_valueprint_priv.h"
#include <unistd.h>

/**
 * @brief Prints a symbol name to stdout
 * @param[in] name Null-terminated string containing the symbol name
 * @return int WR_SUCCESS on success, or error code on write failure
 */
int Writer_NamePrint_print(char* name)
{
    size_t len = 0;  // Length of the name string
    int ret_val; // Return value from write

    if (name == NULL)
    {
        return WR_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    while(name[len] != '\0')// Calculate string length
    {
        len++;
    }  
    ret_val = write(STDOUT_FILENO, name, len);  // Write name to stdout
    if (ret_val != (int)len)
    {
        return (ret_val < 0 ? WR_ERR_WRITE_FAIL : WR_ERR_WRITE_PARTIAL);  // Fail or partial write
    }
    return WR_SUCCESS;  // Success
}