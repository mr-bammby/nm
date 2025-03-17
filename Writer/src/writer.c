/**
 * @file writer.c
 * @brief Basic writer functions for ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file contains basic functions for printing symbol information
 * in ft_nm, formatting and outputting symbol values, flags, and names
 * to stdout with appropriate spacing and newlines.
 */

#include "../inc_pub/writer.h"
#include "../inc_priv/writer_valueprint_priv.h"
#include "../inc_priv/writer_flagprint_priv.h"
#include "../inc_priv/writer_nameprint_priv.h"
#include <unistd.h>

/**
 * @brief Print macros
 */
#define SPACE_STR " "  /**< Single space string */
#define SPACE_LEN 1    /**< Length of space string */
#define NL_STR "\n"    /**< Newline string */
#define NL_LEN 1       /**< Length of newline string */



/**
 * @brief Prints a symbol line to stdout
 * @param[in] line Pointer to the writer_line_t structure containing symbol data
 * @param[in] bit_len Bit length for value formatting (e.g., WRITER_BIT_32 or WRITER_BIT_64)
 * @return int WR_SUCCESS on success, WR_ERR_NULL_INPUT on invalid input,
 *             WR_ERR_WRITE_FAIL on complete write failure, WR_ERR_WRITE_PARTIAL on partial write,
 *             or error code from subsidiary print functions
 */
int Writer_linePrint(const writer_line_t *line, writer_bit_t bit_len)
{
    int ret_val;

    if (line == NULL)
    {
        return WR_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    ret_val = Writer_ValuePrint_print(line->value, (line->sect_head_idx == WRITER_FLAGPRINT_SHIDX_UNDEFINED), bit_len);  // Print symbol value
    if (ret_val == WR_SUCCESS)
    {
        ret_val = write(STDOUT_FILENO, SPACE_STR, SPACE_LEN);  // Add space after value
        if (ret_val != SPACE_LEN)
        {
            return (ret_val < 0 ? WR_ERR_WRITE_FAIL : WR_ERR_WRITE_PARTIAL);  // Fail or partial write
        }
        ret_val = WR_SUCCESS;  // Reset to success if write succeeds
    }
    if (ret_val == WR_SUCCESS)
    {
        ret_val = Writer_FlagPrint_print(line->bind, line->sect_head_idx, line->type);  // Print symbol flags
    }
    if (ret_val == WR_SUCCESS)
    {
        ret_val = write(STDOUT_FILENO, SPACE_STR, SPACE_LEN);  // Add space after flags
        if (ret_val != SPACE_LEN)
        {
            return (ret_val < 0 ? WR_ERR_WRITE_FAIL : WR_ERR_WRITE_PARTIAL);  // Fail or partial write
        }
        ret_val = WR_SUCCESS;  // Reset to success if write succeeds
    }
    if (ret_val == WR_SUCCESS)
    {
        ret_val = Writer_NamePrint_print(line->name);  // Print symbol name
    }  
    if (ret_val == WR_SUCCESS)
    {
        ret_val = write(STDOUT_FILENO, NL_STR, NL_LEN);  // Add newline at end
        if (ret_val != NL_LEN)
        {
            return (ret_val < 0 ? WR_ERR_WRITE_FAIL : WR_ERR_WRITE_PARTIAL);  // Fail or partial write
        }
        ret_val = WR_SUCCESS;  // Reset to success if write succeeds
    }
    return ret_val;  // Return final result
}
