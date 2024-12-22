#include "../ElfParser/inc_pub/elfparser_header.h"
#include "../ElfParser/inc_pub/elfparser_secthead.h"
#include "../ElfParser/inc_pub/elfparser_symtable.h"
#include "../FileHandler/inc_pub/filehandler.h"
#include "../LinkedList/inc_pub/linkedlist.h"
#include "../Writer/inc_pub/writer.h"
#include "../Writer/inc_pub/writer_flagprint.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FT_TRUE     1u
#define FT_FALSE    0u

#define NO_SORT         0u
#define NORMAL_SORT     1u
#define REVERSE_SORT    2u

int lineCmp(const writer_line_t *line1, const writer_line_t *line2);

int main (int argc, char **argv)
{
    source_file_t file = {0};
    elfparser_header_t elf_header = {0};
    elfparser_secthead_t elf_sect_head = {0};
    elfparser_symtable_t elf_symbol_table = {0};
    dl_list_t *head = NULL;
    writer_line_t *new_line;

    unsigned short global_only = FT_FALSE;
    unsigned short undifined_only = FT_FALSE;
    unsigned short sort = NORMAL_SORT;

    int ret;

    // Default target file
    const char *target_file = "a.out";

    // Check if the last argument is a target file or flags
    if (argc > 1 && argv[argc - 1][0] != '-') {
        target_file = argv[argc - 1];
        argc--; // Exclude the target file from flag processing
    }


    // Iterate through all arguments (flags)
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
            // Check all characters in the flag (skip the initial '-')
            for (size_t j = 1; j < strlen(argv[i]); j++) {
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
                        fprintf(stderr, "Error: Unknown flag: %c\n", flag);
                        return (EXIT_FAILURE);
                      
                }
            }
        } else {
            // If it's not a flag (e.g., additional parameter)
            fprintf(stderr, "Error: Unknown parameter or argument: %s\n", argv[i]);
            return (EXIT_FAILURE);
        }
    }

    printf("All flags are valid, and the target file '%s' is executable.\n", target_file);

    FileHandler_structSetup(&file);
    if (FileHandler_fileOpen(&file, target_file) != 0)
    {
        printf("Problem with opening file\n");
        return(2);
    }
    ret = FileHandler_mapGet(&file, 16, 0);
    if (ret)
    {
        printf("Problem with getting a map, Withh err: %d\n", ret);
        return(3);
    }
    ret = ElfParser_Header_identParse(&elf_header, file.map, file.map_len);
    if (ret)
    {
        printf("Problem parsing, With err: %d\n", ret);
    }
    ret = FileHandler_mapGet(&file, ElfParser_Header_sizeGet(&elf_header), 0);
    if (ret)
    {
        printf("Problem with getting a map, Withh err: %d\n", ret);
    }
    ret = ElfParser_Header_parse(&elf_header, file.map, file.map_len);
    if (ret)
    {
        printf("Problem parsing, With err: %d\n", ret);
    }
    ret = FileHandler_mapGet(&file, (elf_header.elf_section_header_entry_num * elf_header.elf_section_header_entry_size), elf_header.elf_section_header_off);
    if (ret)
    {
        printf("Problem with getting a map, Withh err: %d\n", ret);
    }
    ret = ElfParser_SectHead_structSetup(&elf_sect_head, &elf_header);
    if (ret)
    {
        printf("Problem struct setup, With err: %d\n", ret);
    }
    ret = ElfParser_SectHead_parse(&elf_sect_head, file.map, file.map_len);
    if (ret)
    {
        printf("Problem section header parsing, With err: %d\n", ret);
    }
    ret = FileHandler_mapGet(&file, (elf_sect_head.table)[elf_sect_head.string_table_idx].sh_size, (elf_sect_head.table)[elf_sect_head.string_table_idx].sh_offset);
    if (ret)
    {
        printf("Problem with getting a map, Withh err: %d\n", ret);
    }
    ret = ElfParser_SectHead_nameResolve(&elf_sect_head, file.map,  file.map_len);
    if (ret)
    {
        printf("Problem section header name resolving, With err: %d\n", ret);
    }
    int32_t symtab_sect_index = ElfParser_SectHead_byNameFind(&elf_sect_head, ".symtab", 0);
    if  (symtab_sect_index < 0)
    {
        printf("Problem with reding section index, Withh err: %d\n", symtab_sect_index);
        return(1);
    }
    ret = FileHandler_mapGet(&file, elf_sect_head.table[symtab_sect_index].sh_size, elf_sect_head.table[symtab_sect_index].sh_offset);
    if (ret)
    {
        printf("Problem with getting a map, Withh err: %d\n", ret);
    }
    ret = ElfParser_SymTable_structSetup(&elf_symbol_table, &elf_sect_head, symtab_sect_index, &elf_header);
    if (ret)
    {
        printf("Problem struct setup, With err: %d\n", ret);
    }
    ret = ElfParser_SymTable_parse(&elf_symbol_table, file.map, file.map_len);
    if (ret)
    {
        printf("Problem ssymbol table parsing, With err: %d\n", ret);
    }
    ret = FileHandler_mapGet(&file, (elf_sect_head.table)[elf_symbol_table.string_table_idx].sh_size, (elf_sect_head.table)[elf_symbol_table.string_table_idx].sh_offset);
    if (ret)
    {
        printf("Problem with getting a map, Withh err: %d\n", ret);
    }
    ret = ElfParser_SymTable_nameResolve(&elf_symbol_table, file.map,  file.map_len);
    if (ret)
    {
        printf("Problem section header name resolving, With err: %d\n", ret);
    }
    FileHandler_fileClose(&file);
    Writer_FlagPrint_sectionHeadLoad(&elf_sect_head);
    for (int i = 1; i < elf_symbol_table.table_len; i++)
    {
        if (((elf_symbol_table.table)[i].sym_type == ELFPARSER_SYMTABLE_TYPE_SECT) || ((elf_symbol_table.table)[i].sym_type == ELFPARSER_SYMTABLE_TYPE_FILE))
        {
            continue;
        }
        new_line = malloc(sizeof(writer_line_t));
        if (new_line == NULL)
        {
            printf ("Bad malloc \n");
            return(1);
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
                printf("Bad binding val\n");
                return(3);
                break;
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
                printf("Bad type val\n");
                return(3);
                break;
        }
        new_line->sect_head_idx = (elf_symbol_table.table)[i].sym_sect_idx;
        if ((undifined_only == FT_TRUE) && (new_line->sect_head_idx != WRITER_FLAGPRINT_SHIDX_UNDEFINED))
        {
            free(new_line);
            continue;
        }
        new_line->name = (elf_symbol_table.table)[i].sym_name;
        new_line->value = (elf_symbol_table.table)[i].sym_value;
        LinkedList_nodePushFront(&head, new_line);
    }
    if (sort != NO_SORT)
    {
        LinkedList_sort(&head, lineCmp);
    }
    if (sort == NORMAL_SORT)
    {
        for (dl_list_t *node = head; node != NULL; node = node->next)
        {
            Writer_linePrint(node->line);
        }
    }
    else if (head != NULL)
    {
        dl_list_t *node = head;
        for (; node->next != NULL; node = node->next)
        {
            ;
        }
        for (; node != NULL; node = node->prev)
        {
            Writer_linePrint(node->line);
        }

    }
    LinkedList_delete(&head, free);
    ElfParser_SymTable_free(&elf_symbol_table);
    ElfParser_SectHead_free(&elf_sect_head);
    return (EXIT_SUCCESS);
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