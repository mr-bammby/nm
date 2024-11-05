#include "../ElfParser/inc_pub/elfparser_header.h"
#include "../ElfParser/inc_pub/elfparser_secthead.h"
#include "../ElfParser/inc_pub/elfparser_strtable.h"
#include "../FileHandler/inc_pub/filehandler.h"
#include <stdio.h>

int main (int argc, char **argv)
{
    source_file_t file = {0};
    elfparser_header_t elf_header = {0};
    elfparser_secthead_t elf_sect_head = {0};

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
    printf("idx|size|off|name\n");
    for (int i = 0; i < elf_sect_head.table_len; i++)
    {
        printf("%d|%lX|%lX|%s\n", i, (elf_sect_head.table)[i].sh_size, (elf_sect_head.table)[i].sh_offset, (elf_sect_head.table)[i].sh_name);
    }

    ElfParser_SectHead_free(&elf_sect_head);
    FileHandler_fileClose(&file);
}