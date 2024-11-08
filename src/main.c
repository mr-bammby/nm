#include "../ElfParser/inc_pub/elfparser_header.h"
#include "../ElfParser/inc_pub/elfparser_secthead.h"
#include "../ElfParser/inc_pub/elfparser_symtable.h"
#include "../FileHandler/inc_pub/filehandler.h"
#include "../LinkedList/inc_pub/linkedlist.h"
#include "../Writer/inc_pub/writer.h"
#include "../Writer/inc_pub/writer_flagprint.h"


#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
    source_file_t file = {0};
    elfparser_header_t elf_header = {0};
    elfparser_secthead_t elf_sect_head = {0};
    elfparser_symtable_t elf_symbol_table = {0};
    dl_list_t *head = NULL;
    dl_list_t *next_node = NULL;
    writer_line_t *new_line;

    int ret;

    if (argc != 2)
    {
        printf("Wrong number of arguments\n");
        return(1);
    }
    FileHandler_structSetup(&file);
    if (FileHandler_fileOpen(&file, argv[1]) != 0)
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
    printf("MAGIC: 0x%x%c%c%c\n", elf_header.elf_ident.elf_magic_num[0], elf_header.elf_ident.elf_magic_num[1], elf_header.elf_ident.elf_magic_num[2], elf_header.elf_ident.elf_magic_num[3]);
    printf("CLASS: %d\n", elf_header.elf_ident.elf_class);
    printf("DATA: %d\n", elf_header.elf_ident.elf_data);
    printf("VERSION: %d\n", elf_header.elf_ident.elf_version);
    printf("OSABI: %d\n", elf_header.elf_ident.elf_osabi);
    printf("OSABI_VER: %d\n", elf_header.elf_ident.elf_abi_version);
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
    printf("Entry point %ld\n", elf_header.elf_entry);
    printf("Start of section headers %ld\n", elf_header.elf_section_header_off);
    printf("Size of section header entry %d bytes\n", elf_header.elf_section_header_entry_size);
    printf("Num of section headers %d\n", elf_header.elf_section_header_entry_num);
    printf("Section header string table index %d\n", elf_header.elf_section_header_name_idx);
    
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
    printf("---------------------------!--------\n");
    printf("idx|size|off|name|entry_size\n");
    for (int i = 0; i < elf_sect_head.table_len; i++)
    {
        printf("%d|%lX|%lX|%s|%lX\n", i, (elf_sect_head.table)[i].sh_size, (elf_sect_head.table)[i].sh_offset, (elf_sect_head.table)[i].sh_name, (elf_sect_head.table)[i].sh_entsize);
    }
    int32_t symtab_sect_index = ElfParser_SectHead_byNameFind(&elf_sect_head, ".symtab", 0);
    if  (symtab_sect_index < 0)
    {
        printf("Problem with reding section index, Withh err: %d\n", symtab_sect_index);
        return(1);
    }
    printf("Index: %d\n", symtab_sect_index);
    printf("off: %lx, size: %lx\n", (elf_sect_head.table)[symtab_sect_index].sh_offset, (elf_sect_head.table)[symtab_sect_index].sh_size);
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
    printf("off: %lx, size: %lx\n", (elf_sect_head.table)[elf_symbol_table.string_table_idx].sh_offset, (elf_sect_head.table)[elf_symbol_table.string_table_idx].sh_size);
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
    printf("---------------------------!--------\n");
    printf("idx|name|value|binding|section_idx\n");
    for (int i = 0; i < elf_symbol_table.table_len; i++)
    {
        printf("%d|%s|%lX|%x|%d\n", i, (elf_symbol_table.table)[i].sym_name, (elf_symbol_table.table)[i].sym_value, (elf_symbol_table.table)[i].sym_bind, (elf_symbol_table.table)[i].sym_sect_idx);
    }
    
    FileHandler_fileClose(&file);
    printf("Test1\n");
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
        new_line->name = (elf_symbol_table.table)[i].sym_name;
        new_line->value = (elf_symbol_table.table)[i].sym_value;
        LinkedList_nodePushBack(&head, new_line, &next_node, next_node);
        Writer_linePrint(new_line);
    }
    printf("Test5\n");
    
    printf("Test6\n");
    printf("%p\n", next_node);
   /* for (dl_list_t *node = next_node; node != NULL; node = node->prev)
    {
        Writer_linePrint(new_line);
    }*/
    printf("Test7\n");
    LinkedList_delete(&head, free);
    ElfParser_SymTable_free(&elf_symbol_table);
    ElfParser_SectHead_free(&elf_sect_head);
}