/**
 * @file error.h
 * @brief Header file for error handling functions in ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header file declares functions for printing various error messages
 * in the ft_nm program, including errors for bad allocations, invalid options,
 * unrecognized file formats, and system errors.
 */

#ifndef _IG_ERROR_H_
#define _IG_ERROR_H_

/**
 * @brief Prints an error message for memory allocation failure
 * @return int Always returns 1
 */
int Err_Print_BadAlloc(void);

/**
 * @brief Prints an error message for an invalid command-line option
 * @param[in] option The invalid option character
 * @return int Always returns 1
 */
int Err_Print_BadOption(const char* option);

/**
 * @brief Prints an error message for an unrecognized file format
 * @param[in] file_name Name of the file with the bad format
 * @return int Always returns 1
 */
int Err_Print_BadFormat(const char* file_name);

/**
 * @brief Prints an error message based on the current errno value
 * @param[in] file_name Name of the file associated with the error
 * @return int Always returns 1
 */
int Err_Print_Errno(const char* file_name);

#endif /* _IG_ERROR_H_ */
