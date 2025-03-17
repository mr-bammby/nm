/**
 * @file writer_valueprint.c
 * @brief Symbol value printing functions for ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file contains functions for formatting and printing symbol values
 * in hexadecimal format for ft_nm, supporting 32-bit and 64-bit lengths.

 */

#include "../inc_priv/writer_valueprint_priv.h"
#include <unistd.h>

/**
 * @brief Print macros
 */
#define MAX_LEN_64 16         /**< Maximum length for 64-bit value string */
#define LEN_TO_PRINT_64 MAX_LEN_64 /**< Length to print for 64-bit values */
#define UNDEF_VALUE_64 "                " /**< String for undefined 64-bit values */
#define ZEROS_64       "0000000000000000" /**< Zero padding string for 64-bit values */

#define MAX_LEN_32 8          /**< Maximum length for 32-bit value string */
#define LEN_TO_PRINT_32 MAX_LEN_32 /**< Length to print for 32-bit values */
#define UNDEF_VALUE_32 "        " /**< String for undefined 32-bit values */
#define ZEROS_32       "00000000" /**< Zero padding string for 32-bit values */

/**
 * @brief Converts an unsigned 64-bit integer to a hexadecimal string
 * @param[in] num Value to convert
 * @param[in,out] ret_str Pointer to the string buffer; updated to point to the start of the result
 * @param[in] bit_len Bit length (WRITER_VALUEPRINT_32BIT or WRITER_VALUEPRINT_64BIT)
 * @return int8_t Length of the resulting hexadecimal string, or WR_ERR_NULL_INPUT if ret_str is invalid
 */
static int8_t valueprint_ulltoa_hex(uint64_t num, char** ret_str, writer_bit_t bit_len)
{
    char base[] = "0123456789abcdef";  // Hexadecimal digit base
    uint8_t base_size = 16;            // Base 16 for hexadecimal
    uint64_t factor = 1;               // Factor for digit placement
    int8_t num_len = 0;                // Length of the resulting string
    uint8_t next_num;                  // Next digit to process
    short max_len = (bit_len == WRITER_VALUEPRINT_32BIT) ? (MAX_LEN_32) : (MAX_LEN_64);  // Max length based on bit size

    if (ret_str == NULL || *ret_str == NULL)
    {
        return WR_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }

    // Initialize the string with zeros
    for(int i = 0; i < max_len; i++) {
        (*ret_str)[i] = '0';
    }
    (*ret_str)[max_len] = '\0';
    
    do {
        next_num = num % base_size;  // Extract least significant digit
        num -= next_num;
        num /= base_size;            // Shift right by one hex digit
        factor *= base_size;
        max_len--;
        num_len++;
        (*ret_str)[max_len] = base[next_num];  // Place digit in string
    } while ((max_len != 0) && (num != 0));
    *ret_str = &((*ret_str)[max_len]);  // Adjust pointer to start of significant digits
    return num_len;                     // Return length of significant digits
}

/**
 * @brief Prints a symbol value to stdout in hexadecimal format
 * @param[in] value 64-bit value to print
 * @param[in] is_undefined Flag indicating if the value is undefined
 * @param[in] bit_len Bit length (WRITER_VALUEPRINT_32BIT or WRITER_VALUEPRINT_64BIT)
 * @return int WR_SUCCESS on success, WR_ERR_WRITE_FAIL on complete write failure,
 *             WR_ERR_WRITE_PARTIAL on partial write, or WR_ERR_NULL_INPUT from helper
 */
int Writer_ValuePrint_print(uint64_t value, uint8_t is_undefined, writer_bit_t bit_len)
{
    char num_val[((bit_len == WRITER_VALUEPRINT_32BIT) ? (MAX_LEN_32) : (MAX_LEN_64)) + 1];  // Buffer for hex string
    char *num_val_p = num_val;  // Pointer to buffer
    int8_t num_len;             // Length of converted hex string
    int temp;                   // Temporary return value from write
    int expected_len;           // Expected length for write check
    
    if (is_undefined)
    {
        expected_len = (bit_len == WRITER_VALUEPRINT_32BIT) ? LEN_TO_PRINT_32 : LEN_TO_PRINT_64;
        temp = write(STDOUT_FILENO, 
                     ((bit_len == WRITER_VALUEPRINT_32BIT) ? (UNDEF_VALUE_32) : (UNDEF_VALUE_64)), 
                     expected_len);  // Print undefined value
        if (temp != expected_len)
        {
            return (temp < 0 ? WR_ERR_WRITE_FAIL : WR_ERR_WRITE_PARTIAL);  // Fail or partial write
        }
        return WR_SUCCESS;  // Success
    }
    num_len = valueprint_ulltoa_hex(value, &num_val_p, bit_len);  // Convert value to hex
    if (num_len == WR_ERR_NULL_INPUT)
    {
        return num_len;  // Propagate null input error
    }
    expected_len = (bit_len == WRITER_VALUEPRINT_32BIT) ? LEN_TO_PRINT_32 : LEN_TO_PRINT_64;
    temp = write(STDOUT_FILENO, 
                 ((bit_len == WRITER_VALUEPRINT_32BIT) ? (ZEROS_32) : (ZEROS_64)), 
                 expected_len - num_len);  // Print leading zeros
    if (temp != (expected_len - num_len))
    {
        return (temp < 0 ? WR_ERR_WRITE_FAIL : WR_ERR_WRITE_PARTIAL);  // Fail or partial write
    }
    temp = write(STDOUT_FILENO, num_val_p, num_len);  // Print hex value
    if (temp != num_len)
    {
        return (temp < 0 ? WR_ERR_WRITE_FAIL : WR_ERR_WRITE_PARTIAL);  // Fail or partial write
    }
    return WR_SUCCESS;  // Success
}
