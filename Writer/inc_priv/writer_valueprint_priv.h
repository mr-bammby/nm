/**
 * @file writer_valueprint_priv.h
 * @brief Private header for symbol value printing in ft_nm writer module
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header declares functions and types for printing symbol values
 * in hexadecimal format within the ft_nm writer module. It is intended
 * for internal use only by writer module components.
 */

#ifndef _IG_WRITER_VALUEPRINT_PRIV_
#define _IG_WRITER_VALUEPRINT_PRIV_

#include <inttypes.h>
#include "../inc_pub/writer.h"

/**
 * @brief Prints a symbol value to stdout in hexadecimal format
 * @param[in] value 64-bit value to print
 * @param[in] is_undefined Flag indicating if the value is undefined
 * @param[in] bit_len Bit length (e.g., WRITER_BIT_32 or WRITER_BIT_64)
 * @return int WR_SUCCESS on success, WR_ERR_WRITE_FAIL on complete write failure,
 *             WR_ERR_WRITE_PARTIAL on partial write, or WR_ERR_NULL_INPUT on invalid input
 */
int Writer_ValuePrint_print(uint64_t value, uint8_t is_undefined, writer_bit_t bit_len);

#endif /* _IG_WRITER_VALUEPRINT_PRIV_ */