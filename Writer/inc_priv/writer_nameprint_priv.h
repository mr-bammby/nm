/**
 * @file writer_nameprint_priv.h
 * @brief Private header for symbol name printing in ft_nm writer module
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header declares the function for printing symbol names to stdout
 * within the ft_nm writer module. It is intended for internal use only
 * by writer module components.
 */

#ifndef _IG_WRITER_NAMEPRINT_PRIV_
#define _IG_WRITER_NAMEPRINT_PRIV_

/**
 * @brief Prints a symbol name to stdout
 * @param[in] name Null-terminated string containing the symbol name
 * @return int WR_SUCCESS on success, or error code (e.g., WR_ERR_WRITE_FAIL) on write failure
 */
int Writer_NamePrint_print(char* name);

#endif /* _IG_WRITER_NAMEPRINT_PRIV_ */
