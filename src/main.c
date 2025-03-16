#include "../ElfParser/inc_pub/elfparser_header.h"
#include "../ElfParser/inc_pub/elfparser_secthead.h"
#include "../ElfParser/inc_pub/elfparser_symtable.h"
#include "../FileHandler/inc_pub/filehandler.h"
#include "../LinkedList/inc_pub/linkedlist.h"
#include "../Writer/inc_pub/writer.h"
#include "../Writer/inc_pub/writer_flagprint.h"
#include "../inc/error.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define FT_TRUE     1u
#define FT_FALSE    0u

#define NO_SORT         0u
#define NORMAL_SORT     1u
#define REVERSE_SORT    2u

#define RET_OK 0u
#define RET_FILE_ERR 1u
#define RET_PARSE_ERR 2u

int lineCmp(const writer_line_t *line1, const writer_line_t *line2);

unsigned int parseFile(const char* file_name, elfparser_symtable_t *elf_symbol_table, elfparser_secthead_t *elf_sect_head, writer_bit_t *file_bit)
{
    source_file_t file = {0};
    elfparser_header_t elf_header = {0};
    int32_t symtab_sect_index;
    unsigned int ret = RET_OK;

    FileHandler_structSetup(&file);
    ret = FileHandler_fileOpen(&file, file_name);
    if (ret != RET_OK)
    {
        return (RET_FILE_ERR);
    }
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, 16, 0);
        if (ret != RET_OK)
        {
            ret = RET_FILE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = ElfParser_Header_identParse(&elf_header, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
        else
        {
            *file_bit = (elf_header.elf_ident.elf_class == ELFPARSER_HEADER_CLASS_32_BIT) ? (WRITER_VALUEPRINT_32BIT) : (WRITER_VALUEPRINT_64BIT); // set address lenght for writer
        }
    }
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, ElfParser_Header_sizeGet(&elf_header), 0);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = ElfParser_Header_parse(&elf_header, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, (elf_header.elf_section_header_entry_num * elf_header.elf_section_header_entry_size), elf_header.elf_section_header_off);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = ElfParser_SectHead_structSetup(elf_sect_head, &elf_header);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = ElfParser_SectHead_parse(elf_sect_head, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, (elf_sect_head->table)[elf_sect_head->string_table_idx].sh_size, (elf_sect_head->table)[elf_sect_head->string_table_idx].sh_offset);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = ElfParser_SectHead_nameResolve(elf_sect_head, file.map,  file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        symtab_sect_index = ElfParser_SectHead_byNameFind(elf_sect_head, ".symtab", 0);
        if  (symtab_sect_index < 0)
        {
            ret = RET_PARSE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, elf_sect_head->table[symtab_sect_index].sh_size, elf_sect_head->table[symtab_sect_index].sh_offset);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = ElfParser_SymTable_structSetup(elf_symbol_table, elf_sect_head, symtab_sect_index, &elf_header);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = ElfParser_SymTable_parse(elf_symbol_table, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, (elf_sect_head->table)[elf_symbol_table->string_table_idx].sh_size, (elf_sect_head->table)[elf_symbol_table->string_table_idx].sh_offset);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }
    if (ret == RET_OK)
    {
        ret = ElfParser_SymTable_nameResolve(elf_symbol_table, file.map,  file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }
    FileHandler_fileClose(&file);
    return (ret);
}

unsigned int symbol_list_create(dl_list_t **head_p, const elfparser_symtable_t elf_symbol_table, unsigned short global_only, unsigned short undifined_only)
{
    writer_line_t *new_line;
    for (int i = 1; i < elf_symbol_table.table_len; i++)
    {

        if (((elf_symbol_table.table)[i].sym_type == ELFPARSER_SYMTABLE_TYPE_SECT) || ((elf_symbol_table.table)[i].sym_type == ELFPARSER_SYMTABLE_TYPE_FILE))
        {
            continue;
        }
        new_line = malloc(sizeof(writer_line_t));
        if (new_line == NULL)
        {
            return(2);
        }
        switch ((elf_symbol_table.table)[i].sym_bind)
        {
            case (ELFPARSER_SYMTABLE_BIND_LOCAL):
                new_line->bind = WRITER_FLAGPRINT_BIND_LOCAL;
                break;
            case (ELFPARSER_SYMTABLE_BIND_GLOBAL):
                new_line->bind = WRITER_FLAGPRINT_BIND_GLOBAL;
                break;
            case (ELFPARSER_SYMTABLE_BIND_WEAK):
                new_line->bind = WRITER_FLAGPRINT_BIND_WEAK;
                break;
            case (ELFPARSER_SYMTABLE_BIND_GNU_UNIQUE):
                new_line->bind = WRITER_FLAGPRINT_BIND_GNU;
                break;
            default:
                free(new_line);
                return(RET_PARSE_ERR);
        }
        if ((global_only == FT_TRUE) && (new_line->bind != WRITER_FLAGPRINT_BIND_GLOBAL))
        {
            free(new_line);
            continue;
        }
        switch ((elf_symbol_table.table)[i].sym_type)
        {
            case (ELFPARSER_SYMTABLE_TYPE_NOTYPE):
                new_line->type = WRITER_FLAGPRINT_TYPE_NOTYPE;
                break;
            case (ELFPARSER_SYMTABLE_TYPE_OBJECT):
                new_line->type = WRITER_FLAGPRINT_TYPE_OBLJECT;
                break;
            case (ELFPARSER_SYMTABLE_TYPE_FUNC):
                new_line->type = WRITER_FLAGPRINT_TYPE_FUNC;
                break;
            case (ELFPARSER_SYMTABLE_TYPE_SECT):
                new_line->type = WRITER_FLAGPRINT_TYPE_TLS;
                break;
            case (ELFPARSER_SYMTABLE_TYPE_GNU_IFUNC):
                new_line->type = WRITER_FLAGPRINT_TYPE_GNU;
                break;
            default:
                free(new_line);
                return(RET_PARSE_ERR);
        }
        new_line->sect_head_idx = (elf_symbol_table.table)[i].sym_sect_idx;
        if ((undifined_only == FT_TRUE) && (new_line->sect_head_idx != WRITER_FLAGPRINT_SHIDX_UNDEFINED))
        {
            free(new_line);
            continue;
        }
        new_line->name = (elf_symbol_table.table)[i].sym_name;
        new_line->value = (elf_symbol_table.table)[i].sym_value;
        LinkedList_nodePushFront(head_p, new_line);
    }
    return (RET_OK);
}

void symbol_print(const dl_list_t *head, unsigned short sort , writer_bit_t file_bit)
{
    if (sort == NORMAL_SORT)
    {
        for (const dl_list_t *node = head; node != NULL; node = node->next)
        {
            Writer_linePrint(node->line, file_bit);
        }
    }
    else if (head != NULL)
    {
        const dl_list_t *node = head;
        for (; node->next != NULL; node = node->next)
        {
            ;
        }
        for (; node != NULL; node = node->prev)
        {
            Writer_linePrint(node->line, file_bit);
        }
    }
}

int main (int argc, char **argv)
{ 
    elfparser_secthead_t elf_sect_head = {0};
    elfparser_symtable_t elf_symbol_table = {0};
    dl_list_t *head = NULL;
    writer_bit_t file_bit;

    unsigned short global_only = FT_FALSE;
    unsigned short undifined_only = FT_FALSE;
    unsigned short sort = NORMAL_SORT;

    int ret, out = EXIT_SUCCESS;

    // Default target file
    char **target_file = NULL;
    size_t     targat_num = 0;

    // Iterate through all arguments (flags)
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && strlen(argv[i]) > 1)
        {
            // Check all characters in the flag (skip the initial '-')
            for (size_t j = 1; j < strlen(argv[i]); j++)
            {
                char flag = argv[i][j];
                // Call appropriate function for each flag
                switch (flag) {
                    case 'a':
                        Writer_FlagPrint_enableDebug();
                        break;
                    case 'g':
                        global_only = FT_TRUE;
                        break;
                    case 'u':
                        undifined_only = FT_TRUE;
                        break;
                    case 'r':
                        sort = REVERSE_SORT;
                        break;
                    case 'p':
                        sort = NO_SORT;
                        break;
                    default:
                        return (Err_Print_BadOption(&flag));
                }
            }
        }
        else
        {
            targat_num++;
        }
    }
    if (targat_num != 0)
    {
        target_file = malloc(targat_num * sizeof(char *));
        if (target_file != NULL)
        {
            size_t targat_cnt = 0;
            // Iterate through all arguments (flags)
            for (int i = 1; i < argc; i++)
            {
                if (argv[i][0] != '-' || strlen(argv[i]) == 1)
                {
                    target_file[targat_cnt] = argv[i];
                    targat_cnt++;
                    if(targat_cnt == targat_num)
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            return (Err_Print_BadAlloc());
        }
    }
    else
    {
        target_file = malloc(targat_num * sizeof(char *));
        if (target_file != NULL)
        {
            targat_num = 1;
            target_file[0] = "a.out";
        }
        else
        {
            return (Err_Print_BadAlloc());
        }
    }
    for (unsigned int i = 0; i < targat_num; i++)
    {        
        ret = parseFile(target_file[i], &elf_symbol_table, &elf_sect_head, &file_bit);
        out |= ret;
        if (ret == RET_FILE_ERR)
        {
            out |= Err_Print_Errno(target_file[i]);
        }
        else if (ret == RET_PARSE_ERR)
        {
            out |= Err_Print_BadFormat(target_file[i]);
        }
        else
        {
            if (targat_num != 1)
            {
                write(1, "\n", 1);
                write(1, target_file[i], strlen(target_file[i]));
                write(1, ":\n", 2);
            }
            Writer_FlagPrint_sectionHeadLoad(&elf_sect_head);
            ret = symbol_list_create(&head, elf_symbol_table, global_only, undifined_only);
            if (ret == RET_OK)
            {
                if (sort != NO_SORT)
                {
                    LinkedList_sort(&head, lineCmp);
                }
                symbol_print(head, sort, file_bit);
            }
            else if (ret == RET_PARSE_ERR)
            {
                out |= Err_Print_BadFormat(target_file[i]);
            }
            else
            {
                out |= Err_Print_Errno(target_file[i]);
            }
            LinkedList_delete(&head, free);
            ElfParser_SymTable_free(&elf_symbol_table);
            ElfParser_SectHead_free(&elf_sect_head);
        }
    }
    free(target_file);
    return (out);
}

int lineCmp(const writer_line_t *line1, const writer_line_t *line2)
{
    size_t name1_cnt = 0;
    size_t name2_cnt = 0;
    char char1, char2;
    
    while (line1->name[name1_cnt] != '\0')
    {
        if (line1->name[name1_cnt] == '_')
        {
            name1_cnt++;
            continue;
        }
        else if ((line1->name[name1_cnt] >= 'a') && (line1->name[name1_cnt] <= 'z'))
        {
            char1 = line1->name[name1_cnt] - ('a' - 'A');
        }
        else
        {
            char1 = line1->name[name1_cnt];
        }
        if (line2->name[name2_cnt] == '_')
        {
            name2_cnt++;
            continue;
        }
        else if ((line2->name[name2_cnt] >= 'a') && (line2->name[name2_cnt] <= 'z'))
        {
            char2 = line2->name[name2_cnt] - ('a' - 'A');
        }
        else
        {
            char2 = line2->name[name2_cnt];
        }
        if (char1 != char2)
        {
            break;
        }
        name1_cnt++;
        name2_cnt++;
    }
    return(char1 - char2);
}
