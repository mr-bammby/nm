/**
 * @file main.c
 * @brief Main implementation of nm clone utility for ELF file symbol table parsing
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 * 
 * This file contains the main implementation of an nm clone utility that parses
 * ELF files and displays their symbol table information. It supports various
 * command-line options for filtering and sorting symbols.
 */

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

// Boolean definitions
#define FT_TRUE     1u
#define FT_FALSE    0u

// Sorting mode definitions
#define NO_SORT         0u
#define NORMAL_SORT     1u
#define REVERSE_SORT    2u

// Return code definitions
#define RET_OK 0u
#define RET_FILE_ERR 1u
#define RET_PARSE_ERR 2u

// Forward declaration of line comparison function for sorting
int lineCmp(const writer_line_t *line1, const writer_line_t *line2);

/**
 * @brief Parses an ELF file and populates symbol table and section header structures
 * @param[in] file_name Path to the ELF file to parse
 * @param[out] elf_symbol_table Pointer to symbol table structure to populate
 * @param[out] elf_sect_head Pointer to section header structure to populate
 * @param[out] file_bit Pointer to store file bit width (32/64)
 * @return unsigned int RET_OK on success, RET_FILE_ERR for file errors, 
 *         RET_PARSE_ERR for parsing errors
 */
unsigned int parseFile(const char* file_name, elfparser_symtable_t *elf_symbol_table, 
                      elfparser_secthead_t *elf_sect_head, writer_bit_t *file_bit)
{
    source_file_t file = {0};
    elfparser_header_t elf_header = {0};
    int32_t symtab_sect_index;
    unsigned int ret = RET_OK;

    // Initialize file handler structure
    FileHandler_structSetup(&file);
    
    // Attempt to open the file
    ret = FileHandler_fileOpen(&file, file_name);
    if (ret != RET_OK)
    {
        return (RET_FILE_ERR);
    }

    // Get initial file mapping (16 bytes for ELF ident)
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, 16, 0);
        if (ret != RET_OK)
        {
            ret = RET_FILE_ERR;
        }
    }

    // Parse ELF identification header
    if (ret == RET_OK)
    {
        ret = ElfParser_Header_identParse(&elf_header, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
        else
        {
            // Set writer bit width based on ELF class (32-bit or 64-bit)
            *file_bit = (elf_header.elf_ident.elf_class == ELFPARSER_HEADER_CLASS_32_BIT) 
                       ? (WRITER_VALUEPRINT_32BIT) : (WRITER_VALUEPRINT_64BIT);
        }
    }

    // Map full ELF header
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, ElfParser_Header_sizeGet(&elf_header), 0);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }

    // Parse complete ELF header
    if (ret == RET_OK)
    {
        ret = ElfParser_Header_parse(&elf_header, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }

    // Map section header table
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, 
            (elf_header.elf_section_header_entry_num * elf_header.elf_section_header_entry_size),
            elf_header.elf_section_header_off);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }

    // Initialize section header structure
    if (ret == RET_OK)
    {
        ret = ElfParser_SectHead_structSetup(elf_sect_head, &elf_header);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }

    // Parse section headers
    if (ret == RET_OK)
    {
        ret = ElfParser_SectHead_parse(elf_sect_head, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }

    // Map string table for section names
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, 
            (elf_sect_head->table)[elf_sect_head->string_table_idx].sh_size,
            (elf_sect_head->table)[elf_sect_head->string_table_idx].sh_offset);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }

    // Resolve section names
    if (ret == RET_OK)
    {
        ret = ElfParser_SectHead_nameResolve(elf_sect_head, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }

    // Find symbol table section
    if (ret == RET_OK)
    {
        symtab_sect_index = ElfParser_SectHead_byNameFind(elf_sect_head, ".symtab", 0);
        if (symtab_sect_index < 0)
        {
            ret = RET_PARSE_ERR;
        }
    }

    // Map symbol table
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, 
            elf_sect_head->table[symtab_sect_index].sh_size,
            elf_sect_head->table[symtab_sect_index].sh_offset);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }

    // Initialize symbol table structure
    if (ret == RET_OK)
    {
        ret = ElfParser_SymTable_structSetup(elf_symbol_table, elf_sect_head, 
                                           symtab_sect_index, &elf_header);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }

    // Parse symbol table
    if (ret == RET_OK)
    {
        ret = ElfParser_SymTable_parse(elf_symbol_table, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }

    // Map string table for symbol names
    if (ret == RET_OK)
    {
        ret = FileHandler_mapGet(&file, 
            (elf_sect_head->table)[elf_symbol_table->string_table_idx].sh_size,
            (elf_sect_head->table)[elf_symbol_table->string_table_idx].sh_offset);
        if (ret)
        {
            ret = RET_FILE_ERR;
        }
    }

    // Resolve symbol names
    if (ret == RET_OK)
    {
        ret = ElfParser_SymTable_nameResolve(elf_symbol_table, file.map, file.map_len);
        if (ret)
        {
            ret = RET_PARSE_ERR;
        }
    }

    // Clean up file resources
    FileHandler_fileClose(&file);
    return (ret);
}

/**
 * @brief Creates a linked list of symbols from the symbol table
 * @param[in,out] head_p Pointer to head of linked list to populate
 * @param[in] elf_symbol_table Symbol table to process
 * @param[in] global_only Flag to show only global symbols (FT_TRUE/FT_FALSE)
 * @param[in] undifined_only Flag to show only undefined symbols (FT_TRUE/FT_FALSE)
 * @return unsigned int RET_OK on success, error code on failure
 */
unsigned int symbol_list_create(dl_list_t **head_p, const elfparser_symtable_t elf_symbol_table, 
                              unsigned short global_only, unsigned short undifined_only)
{
    writer_line_t *new_line;
    
    // Process each symbol in the table (skip first entry)
    for (int i = 1; i < elf_symbol_table.table_len; i++)
    {
        // Skip section and file symbols
        if (((elf_symbol_table.table)[i].sym_type == ELFPARSER_SYMTABLE_TYPE_SECT) || 
            ((elf_symbol_table.table)[i].sym_type == ELFPARSER_SYMTABLE_TYPE_FILE))
        {
            continue;
        }

        // Allocate memory for new symbol line
        new_line = malloc(sizeof(writer_line_t));
        if (new_line == NULL)
        {
            return(2);  // Memory allocation error
        }

        // Set symbol binding type
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

        // Skip non-global symbols if global_only flag is set
        if ((global_only == FT_TRUE) && (new_line->bind != WRITER_FLAGPRINT_BIND_GLOBAL))
        {
            free(new_line);
            continue;
        }

        // Set symbol type
        switch ((elf_symbol_table.table)[i].sym_type)
        {
            case (ELFPARSER_SYMTABLE_TYPE_NOTYPE):
                new_line->type = WRITER_FLAGPRINT_TYPE_NOTYPE;
                break;
            case (ELFPARSER_SYMTABLE_TYPE_OBJECT):
                new_line->type = WRITER_FLAGPRINT_TYPE_OBJECT;
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

        // Set section header index
        new_line->sect_head_idx = (elf_symbol_table.table)[i].sym_sect_idx;

        // Skip defined symbols if undifined_only flag is set
        if ((undifined_only == FT_TRUE) && 
            (new_line->sect_head_idx != WRITER_FLAGPRINT_SHIDX_UNDEFINED))
        {
            free(new_line);
            continue;
        }

        // Set symbol name and value
        new_line->name = (elf_symbol_table.table)[i].sym_name;
        new_line->value = (elf_symbol_table.table)[i].sym_value;

        // Add to linked list
        LinkedList_nodePushFront(head_p, new_line);
    }
    return (RET_OK);
}

/**
 * @brief Prints symbols from the linked list
 * @param[in] head Head of the symbol list
 * @param[in] sort Sorting mode (NO_SORT, NORMAL_SORT, REVERSE_SORT)
 * @param[in] file_bit File bit width for printing
 */
void symbol_print(const dl_list_t *head, unsigned short sort, writer_bit_t file_bit)
{
    if (sort == NORMAL_SORT)
    {
        // Print symbols in forward order
        for (const dl_list_t *node = head; node != NULL; node = node->next)
        {
            Writer_linePrint(node->line, file_bit);
        }
    }
    else if (head != NULL)
    {
        // Find last node for reverse printing
        const dl_list_t *node = head;
        for (; node->next != NULL; node = node->next)
        {
            ;
        }
        // Print symbols in reverse order
        for (; node != NULL; node = node->prev)
        {
            Writer_linePrint(node->line, file_bit);
        }
    }
}

/**
 * @brief Main entry point for nm clone utility
 * @param[in] argc Number of command-line arguments
 * @param[in] argv Array of command-line arguments
 * @return int Exit status (EXIT_SUCCESS on success, error code on failure)
 */
int main (int argc, char **argv)
{ 
    elfparser_secthead_t elf_sect_head = {0};
    elfparser_symtable_t elf_symbol_table = {0};
    dl_list_t *head = NULL;
    writer_bit_t file_bit;

    // Initialize flags
    unsigned short global_only = FT_FALSE;
    unsigned short undifined_only = FT_FALSE;
    unsigned short sort = NORMAL_SORT;

    int ret, out = EXIT_SUCCESS;

    // Default target file
    char **target_file = NULL;
    size_t target_num = 0;

    // Process command-line arguments for flags
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && strlen(argv[i]) > 1)
        {
            // Process each character in flag string
            for (size_t j = 1; j < strlen(argv[i]); j++)
            {
                char flag = argv[i][j];
                switch (flag) {
                    case 'a':  // Enable debug output
                        Writer_FlagPrint_enableDebug();
                        break;
                    case 'g':  // Show only global symbols
                        global_only = FT_TRUE;
                        break;
                    case 'u':  // Show only undefined symbols
                        undifined_only = FT_TRUE;
                        break;
                    case 'r':  // Reverse sort order
                        sort = REVERSE_SORT;
                        break;
                    case 'p':  // No sorting
                        sort = NO_SORT;
                        break;
                    default:
                        return (Err_Print_BadOption(&flag));
                }
            }
        }
        else
        {
            target_num++;  // Count non-flag arguments as target files
        }
    }

    // Allocate memory for target file list
    if (target_num != 0)
    {
        target_file = malloc(target_num * sizeof(char *));
        if (target_file != NULL)
        {
            size_t targat_cnt = 0;
            // Collect target file names
            for (int i = 1; i < argc; i++)
            {
                if (argv[i][0] != '-' || strlen(argv[i]) == 1)
                {
                    target_file[targat_cnt] = argv[i];
                    targat_cnt++;
                    if(targat_cnt == target_num)
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
        // Use default file "a.out" if no files specified
        target_file = malloc(target_num * sizeof(char *));
        if (target_file != NULL)
        {
            target_num = 1;
            target_file[0] = "a.out";
        }
        else
        {
            return (Err_Print_BadAlloc());
        }
    }

    // Process each target file
    for (unsigned int i = 0; i < target_num; i++)
    {        
        ret = parseFile(target_file[i], &elf_symbol_table, &elf_sect_head, &file_bit);
        out |= ret;
        
        // Handle parsing errors
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
            // Print file name header for multiple files
            if (target_num != 1)
            {
                write(1, "\n", 1);
                write(1, target_file[i], strlen(target_file[i]));
                write(1, ":\n", 2);
            }
            
            // Load section header information
            Writer_FlagPrint_sectionHeadLoad(&elf_sect_head);
            
            // Create symbol list
            ret = symbol_list_create(&head, elf_symbol_table, global_only, undifined_only);
            if (ret == RET_OK)
            {
                // Sort symbols if required
                if (sort != NO_SORT)
                {
                    LinkedList_sort(&head, lineCmp);
                }
                // Print symbols
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
            
            // Clean up resources
            LinkedList_delete(&head, free);
            ElfParser_SymTable_free(&elf_symbol_table);
            ElfParser_SectHead_free(&elf_sect_head);
        }
    }
    
    // Clean up target file list
    free(target_file);
    return (out);
}

/**
 * @brief Compares two symbol lines for sorting
 * @param[in] line1 First symbol line to compare
 * @param[in] line2 Second symbol line to compare
 * @return int Negative if line1 < line2, positive if line1 > line2, 0 if equal
 */
int lineCmp(const writer_line_t *line1, const writer_line_t *line2)
{
    size_t name1_cnt = 0;
    size_t name2_cnt = 0;
    char char1, char2;
    
    // Compare symbol names, ignoring underscores and converting lowercase to uppercase
    while (line1->name[name1_cnt] != '\0')
    {
        // Skip underscores in first name
        if (line1->name[name1_cnt] == '_')
        {
            name1_cnt++;
            continue;
        }
        // Convert lowercase to uppercase
        else if ((line1->name[name1_cnt] >= 'a') && (line1->name[name1_cnt] <= 'z'))
        {
            char1 = line1->name[name1_cnt] - ('a' - 'A');
        }
        else
        {
            char1 = line1->name[name1_cnt];
        }

        // Skip underscores in second name
        if (line2->name[name2_cnt] == '_')
        {
            name2_cnt++;
            continue;
        }
        // Convert lowercase to uppercase
        else if ((line2->name[name2_cnt] >= 'a') && (line2->name[name2_cnt] <= 'z'))
        {
            char2 = line2->name[name2_cnt] - ('a' - 'A');
        }
        else
        {
            char2 = line2->name[name2_cnt];
        }

        // Compare characters
        if (char1 != char2)
        {
            break;
        }
        name1_cnt++;
        name2_cnt++;
    }
    return(char1 - char2);
}
