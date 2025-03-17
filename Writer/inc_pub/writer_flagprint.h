/**
 * @file writer_flagprint.h
 * @brief Public header for flag printing utilities in the ft_nm writer module
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header defines the public interface for the writer module of the ft_nm
 * clone, responsible for printing flags and related information derived from
 * ELF section and symbol table data. It provides enumerations for symbol binding
 * and type flags, special section indices, and functions to manage section header
 * data and debug output.
 *
 * @section dependencies Dependencies
 * - elfparser_secthead.h: For elfparser_secthead_t structure
 * - elfparser_symtable.h: For ELFPARSER_SYMTABLE_* constants
 *
 * @section enums Enumerations
 * - writer_flagprint_bind_e: Symbol binding types for flag printing
 * - writer_flagprint_type_e: Symbol types for flag printing
 *
 * @section macros Macros
 * - WRITER_FLAGPRINT_SHIDX_*: Special section index values
 */

#ifndef _IG_WRITER_FLAGPRINT_
#define _IG_WRITER_FLAGPRINT_

#include "../../ElfParser/inc_pub/elfparser_secthead.h" // For elfparser_secthead_t
#include "../../ElfParser/inc_pub/elfparser_symtable.h"  // For ELFPARSER_SYMTABLE_* constants

/**
 * @brief Enumeration of symbol binding types for flag printing
 * @enum writer_flagprint_bind_e
 * Maps to ELFPARSER_SYMTABLE_BIND_* values for consistency with libelfparser.
 */
typedef enum
{
    WRITER_FLAGPRINT_BIND_LOCAL  = ELFPARSER_SYMTABLE_BIND_LOCAL,      /**< Local symbol binding (STB_LOCAL) */
    WRITER_FLAGPRINT_BIND_GLOBAL = ELFPARSER_SYMTABLE_BIND_GLOBAL,     /**< Global symbol binding (STB_GLOBAL) */
    WRITER_FLAGPRINT_BIND_WEAK   = ELFPARSER_SYMTABLE_BIND_WEAK,       /**< Weak symbol binding (STB_WEAK) */
    WRITER_FLAGPRINT_BIND_GNU    = ELFPARSER_SYMTABLE_BIND_GNU_UNIQUE  /**< GNU unique symbol binding (STB_GNU_UNIQUE) */
} writer_flagprint_bind_e;

/**
 * @brief Enumeration of symbol types for flag printing
 * @enum writer_flagprint_type_e
 * Maps to ELFPARSER_SYMTABLE_TYPE_* values for consistency with libelfparser.
 */
typedef enum
{
    WRITER_FLAGPRINT_TYPE_NOTYPE  = ELFPARSER_SYMTABLE_TYPE_NOTYPE,    /**< Unspecified symbol type (STT_NOTYPE) */
    WRITER_FLAGPRINT_TYPE_OBJECT  = ELFPARSER_SYMTABLE_TYPE_OBJECT,    /**< Data object symbol type (STT_OBJECT) */
    WRITER_FLAGPRINT_TYPE_FUNC    = ELFPARSER_SYMTABLE_TYPE_FUNC,      /**< Function symbol type (STT_FUNC) */
    WRITER_FLAGPRINT_TYPE_SECT    = ELFPARSER_SYMTABLE_TYPE_SECT,      /**< Section symbol type (STT_SECTION) */
    WRITER_FLAGPRINT_TYPE_FILE    = ELFPARSER_SYMTABLE_TYPE_FILE,      /**< File name symbol type (STT_FILE) */
    WRITER_FLAGPRINT_TYPE_COMMON  = ELFPARSER_SYMTABLE_TYPE_COMMON,    /**< Common data symbol type (STT_COMMON) */
    WRITER_FLAGPRINT_TYPE_TLS     = ELFPARSER_SYMTABLE_TYPE_TLS,       /**< Thread-local storage symbol type (STT_TLS) */
    WRITER_FLAGPRINT_TYPE_GNU     = ELFPARSER_SYMTABLE_TYPE_GNU_IFUNC  /**< GNU indirect function symbol type (STT_GNU_IFUNC) */
} writer_flagprint_type_e;

/* Special Section Index Values (st_shndx) */
#define WRITER_FLAGPRINT_SHIDX_UNDEFINED  0u      /**< Undefined section index (SHN_UNDEF) */
#define WRITER_FLAGPRINT_SHIDX_ABSOLUTE   0xfff1u /**< Absolute value section index (SHN_ABS) */
#define WRITER_FLAGPRINT_SHIDX_COMMON     0xfff2u /**< Common symbol section index (SHN_COMMON) */

/**
 * @brief Loads section header data for flag printing
 * @param[in] sect_head Pointer to the section header structure from libelfparser
 */
void Writer_FlagPrint_sectionHeadLoad(const elfparser_secthead_t *sect_head);

/**
 * @brief Unloads section header data, resetting internal state
 */
void Writer_FlagPrint_sectionHeadUnload(void);

/**
 * @brief Enables debug output for flag printing
 */
void Writer_FlagPrint_enableDebug(void);

/**
 * @brief Disables debug output for flag printing
 */
void Writer_FlagPrint_disableDebug(void);

#endif /* _IG_WRITER_FLAGPRINT_ */