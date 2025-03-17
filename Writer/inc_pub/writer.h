/**
 * @file writer.h
 * @brief Main public header for the ft_nm writer module
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header provides the public interface for the ft_nm writer module, which
 * handles printing symbol information to stdout. It includes error codes, data
 * structures for symbol lines, bit length specifications, and the primary printing
 * function.
 */

#ifndef _IG_WRITER_H_
#define _IG_WRITER_H_

#include "writer_flagprint.h"  // For writer_flagprint_bind_e, writer_flagprint_type_e

/**
 * @brief Error codes for writer operations
 */
enum Writer_Error {
    WR_SUCCESS = 0,          /**< Success */
    WR_ERR_NULL_INPUT = -1,  /**< Invalid input (NULL pointer) */
    WR_ERR_WRITE_FAIL = -2,  /**< Write operation failed completely */
    WR_ERR_WRITE_PARTIAL = -3 /**< Write operation completed partially */
};

/**
 * @brief Structure representing a symbol line for printing
 */
typedef struct writre_line_s
{
    writer_flagprint_bind_e bind;    /**< Symbol binding type (e.g., WRITER_FLAGPRINT_BIND_WEAK) */
    writer_flagprint_type_e type;    /**< Symbol type (e.g., WRITER_FLAGPRINT_TYPE_OBJECT) */
    uint16_t sect_head_idx;          /**< Section header index for the symbol */
    char *name;                      /**< Pointer to the symbol name (null-terminated) */
    uint64_t value;                  /**< Symbol value (32-bit or 64-bit) */
} writer_line_t;

/**
 * @brief Enumeration of bit lengths for symbol value printing
 */
typedef enum 
{
    WRITER_VALUEPRINT_32BIT = 0U, /**< 32-bit value printing */
    WRITER_VALUEPRINT_64BIT = 1U  /**< 64-bit value printing */
} writer_bit_t;

/**
 * @brief Prints a symbol line to stdout
 * @param[in] line Pointer to the writer_line_t structure containing symbol data
 * @param[in] bit_len Bit length for value formatting (WRITER_VALUEPRINT_32BIT or WRITER_VALUEPRINT_64BIT)
 * @return int WR_SUCCESS on success, WR_ERR_NULL_INPUT on invalid input,
 *             WR_ERR_WRITE_FAIL on complete write failure, WR_ERR_WRITE_PARTIAL on partial write
 */
int Writer_linePrint(const writer_line_t *line, writer_bit_t bit_len);

#endif /* _IG_WRITER_H_ */
