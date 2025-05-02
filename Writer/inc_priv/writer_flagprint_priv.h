/**
 * @file writer_flagprint_priv.h
 * @brief Private header for symbol flag printing in ft_nm writer module
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header defines constants and declares functions for printing symbol flags
 * within the ft_nm writer module. It is intended for internal use only by writer
 * module components.
 */

#ifndef _IG_WRITER_FLAGPRINT_PRIV_
#define _IG_WRITER_FLAGPRINT_PRIV_

#include "../inc_pub/writer_flagprint.h"

#define FLAGPRINT_FLAG_ABSOLUTE_GLOBAL      "A" /**< Flag for absolute symbols global (reserved section index) */
#define FLAGPRINT_FLAG_ABSOLUTE_LOCAL       "a" /**< Flag for absolute symbols local(reserved section index) */
#define FLAGPRINT_FLAG_BSS_GLOBAL           "B" /**< Flag for global BSS section symbols (.bss) */
#define FLAGPRINT_FLAG_BSS_LOCAL            "b" /**< Flag for local BSS section symbols (.bss) */
#define FLAGPRINT_FLAG_COMMON               "C" /**< Flag for common symbols (reserved section index) */
#define FLAGPRINT_FLAG_DATA_GLOBAL          "D" /**< Flag for global data section symbols (.data, .data1, etc.) */
#define FLAGPRINT_FLAG_DATA_LOCAL           "d" /**< Flag for local data section symbols (.data, .data1, etc.) */
#define FLAGPRINT_FLAG_INDIRECT             "I" /**< Flag for GNU-specific indirect symbols */
#define FLAGPRINT_FLAG_GNU_TYPE             "i" /**< Flag for GNU-specific symbol type */
#define FLAGPRINT_FLAG_DEBUG_GLOBAL         "N" /**< Flag for debug section symbols global  (.debug) */
#define FLAGPRINT_FLAG_DEBUG_LOCAL          "n" /**< Flag for debug section symbols local  (.debug) */
#define FLAGPRINT_FLAG_RODATA_GLOBAL        "R" /**< Flag for global read-only data section symbols (.rodata, .rodata1, etc.) */
#define FLAGPRINT_FLAG_RODATA_LOCAL         "r" /**< Flag for local read-only data section symbols (.rodata, .rodata1, etc.) */
#define FLAGPRINT_FLAG_CODE_GLOBAL          "T" /**< Flag for global code section symbols (.text) */
#define FLAGPRINT_FLAG_CODE_LOCAL           "t" /**< Flag for local code section symbols (.text) */
#define FLAGPRINT_FLAG_UNDEF                "U" /**< Flag for undefined symbols (reserved section index) */
#define FLAGPRINT_FLAG_GNU_BIND             "u" /**< Flag for GNU-specific symbol binding */
#define FLAGPRINT_FLAG_WEAK_OBJECT          "V" /**< Flag for weak object symbols (reserved binding) */
#define FLAGPRINT_FLAG_WEAK_OBJECT_UNDEF    "v" /**< Flag for undefined weak object symbols */
#define FLAGPRINT_FLAG_WEAK                 "W" /**< Flag for weak symbols (reserved binding) */
#define FLAGPRINT_FLAG_WEAK_UNDEF           "w" /**< Flag for undefined weak symbols */
#define FLAGPRINT_FLAG_UNKNOW               "?" /**< Default flag for unknown symbol attributes */

#define FLAGPRINT_FLAG_LEN 1u /**< Length of flag strings (all are 1 character) */

#define FLAGPRINT_PREFIX_DEBUG ".debug" /**< Prefix for debug section names */
#define FLAGPRINT_PREFIX_DEBUG_LEN 6u /**< Length of debug section prefix */

/**
 * @brief Prints a symbol flag to stdout
 * @param[in] bind Symbol binding type
 * @param[in] symbol_shidx Section header index for the symbol
 * @param[in] type Symbol type
 * @return int WR_SUCCESS on success, WR_ERR_NULL_INPUT if section table is NULL,
 *             WR_ERR_WRITE_FAIL on complete write failure or index out of bounds,
 *             WR_ERR_WRITE_PARTIAL on partial write
 */
int Writer_FlagPrint_print(writer_flagprint_bind_e bind, uint16_t symbol_shidx, writer_flagprint_type_e type);

#endif /* _IG_WRITER_FLAGPRINT_PRIV_ */
