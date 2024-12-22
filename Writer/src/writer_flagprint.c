#include "../inc_pub/writer_flagprint.h"
#include "../inc_priv/writer_flagprint_priv.h"
#include <unistd.h>
#include <stdio.h>

#define NO_PRINT    0u
#define PRINT       1u

const elfparser_secthead_t *g_sect_head_table = NULL;
unsigned short debug_print = NO_PRINT;

static int flagprint_strNCmp(const char *s1, const char *s2, size_t n)
{
    unsigned int i  = 0;

	if (!s1 && !s2)
    {
		return (-1);
    }
    if (s1 == s2)
    {
        return (0);
    }

	while ((i < n) && (*s1 != '\0'))
	{
		if (*s1 != *s2)
        {
            return(((int)*s1) -  ((int)*s2));
        }
		s1++;
        s2++;
        i++;
	}
    
	return (((int16_t)*s1) -  ((int16_t)*s2));
}

void Writer_FlagPrint_sectionHeadLoad(const elfparser_secthead_t *sect_head)
{
    g_sect_head_table = sect_head;
}

void Writer_FlagPrint_sectionHeadload(void)
{
    g_sect_head_table = NULL;
}

void Writer_FlagPrint_enableDebug(void)
{
    debug_print = PRINT;
}

void Writer_FlagPrint_disableDebug(void)
{
    debug_print = NO_PRINT;
}

int Writer_FlagPrint_print(writer_flagprint_bind_e bind, uint16_t symbol_shidx, writer_flagprint_type_e type)
{
    if (g_sect_head_table == NULL)
    {
        return (-1);
    }
    if (bind == WRITER_FLAGPRINT_BIND_WEAK)
    {
        if (type == WRITER_FLAGPRINT_TYPE_OBLJECT)
        {
            write(STDOUT_FILENO, (symbol_shidx == WRITER_FLAGPRINT_SHIDX_UNDEFINED)?(FLAGPRINT_FLAG_WEAK_OBJECT_UNDEF):(FLAGPRINT_FLAG_WEAK_OBJECT), FLAGPRINT_FLAG_LEN);
        }
        else
        {
            write(STDOUT_FILENO, (symbol_shidx == WRITER_FLAGPRINT_SHIDX_UNDEFINED)?(FLAGPRINT_FLAG_WEAK_UNDEF):(FLAGPRINT_FLAG_WEAK), FLAGPRINT_FLAG_LEN);           
        }
        return (0);
    }
    else if(bind == WRITER_FLAGPRINT_BIND_GNU)
    {
        write(STDOUT_FILENO, FLAGPRINT_FLAG_GNU_BIND, FLAGPRINT_FLAG_LEN);
        return (0);
    }
    if (type == WRITER_FLAGPRINT_TYPE_GNU)
    {
        write(STDOUT_FILENO, FLAGPRINT_FLAG_GNU_TYPE, FLAGPRINT_FLAG_LEN);
        return (0);
    }
    if (symbol_shidx == WRITER_FLAGPRINT_SHIDX_ABSOLUTE)
    {
        write(STDOUT_FILENO, FLAGPRINT_FLAG_ABSOLUTE, FLAGPRINT_FLAG_LEN);
        return (0);
    }
    else if (symbol_shidx == WRITER_FLAGPRINT_SHIDX_COMMON)
    {
        write(STDOUT_FILENO, FLAGPRINT_FLAG_COMMON, FLAGPRINT_FLAG_LEN);
        return (0);
    }
    else if (symbol_shidx == WRITER_FLAGPRINT_SHIDX_UNDEFINED)
    {
        write(STDOUT_FILENO, FLAGPRINT_FLAG_UNDEF, FLAGPRINT_FLAG_LEN);
        return (0);
    }
    if (symbol_shidx >= g_sect_head_table->table_len)
    {
        return(-2);
    }
    for (uint8_t i = 0; i < FLAGPRINT_SH_NAME_DATA_ARR_LEN; i++)
    {
        if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_DATA_ARR[i], SIZE_MAX) == 0)
        {
            write(STDOUT_FILENO, (bind == WRITER_FLAGPRINT_BIND_LOCAL)?(FLAGPRINT_FLAG_DATA_LOCAL):(FLAGPRINT_FLAG_DATA_GLOBAL), FLAGPRINT_FLAG_LEN);
            return (0);
        }
    }
    for (uint8_t i = 0; i < FLAGPRINT_SH_NAME_RODATA_ARR_LEN; i++)
    {
        if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_RODATA_ARR[i], SIZE_MAX) == 0)
        {
            write(STDOUT_FILENO, (bind == WRITER_FLAGPRINT_BIND_LOCAL)?(FLAGPRINT_FLAG_RODATA_LOCAL):(FLAGPRINT_FLAG_RODATA_GLOBAL), FLAGPRINT_FLAG_LEN);
            return (0);
        }
    }
    for (uint8_t i = 0; i < FLAGPRINT_SH_NAME_CODE_ARR_LEN; i++)
    {
        if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_CODE_ARR[i], SIZE_MAX) == 0)
        {
            write(STDOUT_FILENO, (bind == WRITER_FLAGPRINT_BIND_LOCAL)?(FLAGPRINT_FLAG_CODE_LOCAL):(FLAGPRINT_FLAG_CODE_GLOBAL), FLAGPRINT_FLAG_LEN);
            return (0);
        }
    }
    if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_BSS, SIZE_MAX) == 0)
    {
        write(STDOUT_FILENO, (bind == WRITER_FLAGPRINT_BIND_LOCAL)?(FLAGPRINT_FLAG_BSS_LOCAL):(FLAGPRINT_FLAG_BSS_GLOBAL), FLAGPRINT_FLAG_LEN);
        return (0);
    }
    else if (flagprint_strNCmp(g_sect_head_table->table[symbol_shidx].sh_name, FLAGPRINT_SH_NAME_DEBUG, SIZE_MAX) == 0)
    {
            write(STDOUT_FILENO, FLAGPRINT_FLAG_DEBUGGING, FLAGPRINT_FLAG_LEN);
        return (0);
    }
    write(STDOUT_FILENO, FLAGPRINT_FLAG_UNKNOW, FLAGPRINT_FLAG_LEN);
    write(STDOUT_FILENO, g_sect_head_table->table[symbol_shidx].sh_name, 20);
    return (0);
}