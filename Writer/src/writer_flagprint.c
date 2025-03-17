/**
 * @file writer_flagprint.c
 * @brief Symbol flag printing functions for ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file contains functions for printing symbol flags to stdout
 * in the ft_nm writer module, based on binding, section index, and type.
 * It supports section header table lookups and debug flag toggling.
 */

#include "../inc_pub/writer_flagprint.h"
#include "../inc_priv/writer_flagprint_priv.h" 
#include "../inc_pub/writer.h"
#include <unistd.h>
#include <string.h>

/**
 * @brief Print macros
 */
#define NO_PRINT    0u  /**< Disable debug printing */
#define PRINT       1u  /**< Enable debug printing */
#define FLAGPRINT_STRNCMP_EQUAL     0   /**< Strings are equal up to n or null-terminated */
#define FLAGPRINT_STRNCMP_BOTH_NULL -1  /**< Both input strings are NULL */

#define SECTION_PRINT NO_PRINT

const elfparser_secthead_t *g_sect_head_table = NULL; /* Global pointer to the section header table */
unsigned short debug_print = NO_PRINT; /* Global flag for enabling/disabling debug output */

/**
 * @brief Compares two strings up to a maximum length
 * @param[in] s1 First string
 * @param[in] s2 Second string
 * @param[in] n Maximum number of characters to compare
 * @return int FLAGPRINT_STRNCMP_EQUAL if strings match, FLAGPRINT_STRNCMP_BOTH_NULL if both NULL,
 *             or difference between characters at first mismatch
 */
static int flagprint_strNCmp(const char *s1, const char *s2, size_t n)
{
    unsigned int i  = 0;  // Character index

    if (!s1 && !s2)
    {
        return FLAGPRINT_STRNCMP_BOTH_NULL;  // Both NULL, treat as error
    }
    if (s1 == s2)
    {
        return FLAGPRINT_STRNCMP_EQUAL;  // Same pointer, equal
    }

    while ((i < n) && (*s1 != '\0'))
    {
        if (*s1 != *s2)
        {
            return ((int)*s1) - ((int)*s2);  // Return difference at mismatch
        }
        s1++;
        s2++;
        i++;
    }
    
    return ((int16_t)*s1) - ((int16_t)*s2);  // Compare final characters or null terminators
}

/**
 * @brief Loads the section header table for flag printing
 * @param[in] sect_head Pointer to the section header table
 */
void Writer_FlagPrint_sectionHeadLoad(const elfparser_secthead_t *sect_head)
{
    g_sect_head_table = sect_head;  // Set global section header table
}

/**
 * @brief Clears the section header table
 */
void Writer_FlagPrint_sectionHeadUnload(void)
{
    g_sect_head_table = NULL;  // Clear global section header table
}

/**
 * @brief Enables debug flag printing
 */
void Writer_FlagPrint_enableDebug(void)
{
    debug_print = PRINT;  // Set debug flag to enabled
}

/**
 * @brief Disables debug flag printing
 */
void Writer_FlagPrint_disableDebug(void)
{
    debug_print = NO_PRINT;  // Set debug flag to disabled
}

/**
 * @brief Prints a symbol flag to stdout
 * @param[in] bind Symbol binding type (e.g., WRITER_FLAGPRINT_BIND_WEAK)
 * @param[in] symbol_shidx Section header index for the symbol
 * @param[in] type Symbol type (e.g., WRITER_FLAGPRINT_TYPE_GNU)
 * @return int WR_SUCCESS on success, WR_ERR_NULL_INPUT if section table is NULL,
 *             WR_ERR_WRITE_FAIL on complete write failure or index out of bounds,
 *             WR_ERR_WRITE_PARTIAL on partial write
 */
int Writer_FlagPrint_print(writer_flagprint_bind_e bind, uint16_t symbol_shidx, writer_flagprint_type_e type)
{
    const char *flag_str;  // Flag string to print
    int ret_val;           // Return value from write

    if (g_sect_head_table == NULL)
    {
        return WR_ERR_NULL_INPUT;  // Section table not loaded
    }
    if (bind == WRITER_FLAGPRINT_BIND_WEAK)
    {
        flag_str = (type == WRITER_FLAGPRINT_TYPE_OBJECT) ?  // Fixed typo from OBLJECT
                   (symbol_shidx == WRITER_FLAGPRINT_SHIDX_UNDEFINED ? FLAGPRINT_FLAG_WEAK_OBJECT_UNDEF : FLAGPRINT_FLAG_WEAK_OBJECT) :
                   (symbol_shidx == WRITER_FLAGPRINT_SHIDX_UNDEFINED ? FLAGPRINT_FLAG_WEAK_UNDEF : FLAGPRINT_FLAG_WEAK);
    }
    else if (bind == WRITER_FLAGPRINT_BIND_GNU)
    {
        flag_str = FLAGPRINT_FLAG_GNU_BIND;
    }
    else if (type == WRITER_FLAGPRINT_TYPE_GNU)
    {
        flag_str = FLAGPRINT_FLAG_GNU_TYPE;
    }
    else if (symbol_shidx == WRITER_FLAGPRINT_SHIDX_ABSOLUTE)
    {
        flag_str = FLAGPRINT_FLAG_ABSOLUTE;
    }
    else if (symbol_shidx == WRITER_FLAGPRINT_SHIDX_COMMON)
    {
        flag_str = FLAGPRINT_FLAG_COMMON;
    }
    else if (symbol_shidx == WRITER_FLAGPRINT_SHIDX_UNDEFINED)
    {
        flag_str = FLAGPRINT_FLAG_UNDEF;
    }
    else
    {
        if (symbol_shidx >= g_sect_head_table->table_len)
        {
            return WR_ERR_WRITE_FAIL;  // Index out of bounds, repurposed as write-related error
        }
        for (uint8_t i = 0; i < FLAGPRINT_SH_NAME_DATA_ARR_LEN; i++)
        {
            if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_DATA_ARR[i], SIZE_MAX) == FLAGPRINT_STRNCMP_EQUAL)
            {
                flag_str = (bind == WRITER_FLAGPRINT_BIND_LOCAL) ? FLAGPRINT_FLAG_DATA_LOCAL : FLAGPRINT_FLAG_DATA_GLOBAL;
                goto print_flag;
            }
        }
        for (uint8_t i = 0; i < FLAGPRINT_SH_NAME_RODATA_ARR_LEN; i++)
        {
            if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_RODATA_ARR[i], SIZE_MAX) == FLAGPRINT_STRNCMP_EQUAL)
            {
                flag_str = (bind == WRITER_FLAGPRINT_BIND_LOCAL) ? FLAGPRINT_FLAG_RODATA_LOCAL : FLAGPRINT_FLAG_RODATA_GLOBAL;
                goto print_flag;
            }
        }
        for (uint8_t i = 0; i < FLAGPRINT_SH_NAME_CODE_ARR_LEN; i++)
        {
            if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_CODE_ARR[i], SIZE_MAX) == FLAGPRINT_STRNCMP_EQUAL)
            {
                flag_str = (bind == WRITER_FLAGPRINT_BIND_LOCAL) ? FLAGPRINT_FLAG_CODE_LOCAL : FLAGPRINT_FLAG_CODE_GLOBAL;
                goto print_flag;
            }
        }
        for (uint8_t i = 0; i < FLAGPRINT_SH_NAME_BSS_ARR_LEN; i++)
        {
            if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_BSS_ARR[i], SIZE_MAX) == FLAGPRINT_STRNCMP_EQUAL)
            {
                flag_str = (bind == WRITER_FLAGPRINT_BIND_LOCAL) ? FLAGPRINT_FLAG_BSS_LOCAL : FLAGPRINT_FLAG_BSS_GLOBAL;
                goto print_flag;
            }
        }
        if (debug_print == PRINT)
        {
            for (uint8_t i = 0; i < FLAGPRINT_SH_NAME_DEBUG_ARR_LEN; i++)
            {
                if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_DEBUG_ARR[i], SIZE_MAX) == FLAGPRINT_STRNCMP_EQUAL)
                {
                    flag_str = FLAGPRINT_FLAG_DEBUG;
                    goto print_flag;
                }
            }
        }
        flag_str = FLAGPRINT_FLAG_UNKNOW;  // Default to unknown
    }

print_flag:
    ret_val = write(STDOUT_FILENO, flag_str, FLAGPRINT_FLAG_LEN);  // Print selected flag
    if (ret_val != FLAGPRINT_FLAG_LEN)
    {
        return (ret_val < 0 ? WR_ERR_WRITE_FAIL : WR_ERR_WRITE_PARTIAL);  // Fail or partial write
    }
    if (SECTION_PRINT == PRINT && symbol_shidx < g_sect_head_table->table_len)  // Debug print if enabled
    {
        size_t name_len = strlen(g_sect_head_table->table[symbol_shidx].sh_name);
        ret_val = write(STDOUT_FILENO, g_sect_head_table->table[symbol_shidx].sh_name, name_len);
        if (ret_val != (int)name_len)
        {
            return (ret_val < 0 ? WR_ERR_WRITE_FAIL : WR_ERR_WRITE_PARTIAL);  // Fail or partial write
        }
    }
    return WR_SUCCESS;  // Success
}
