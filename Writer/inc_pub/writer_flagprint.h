#ifndef _IG_WRITER_FLAGPRINT_
#define _IG_WRITER_FLAGPRINT_

#include "../../ElfParser/inc_pub/elfparser_secthead.h"
#include "../../ElfParser/inc_pub/elfparser_symtable.h"

typedef enum
{
    WRITER_FLAGPRINT_BIND_LOCAL     = ELFPARSER_SYMTABLE_BIND_LOCAL,
    WRITER_FLAGPRINT_BIND_GLOBAL    = ELFPARSER_SYMTABLE_BIND_GLOBAL,
    WRITER_FLAGPRINT_BIND_WEAK      = ELFPARSER_SYMTABLE_BIND_WEAK,
    WRITER_FLAGPRINT_BIND_GNU       = ELFPARSER_SYMTABLE_BIND_GNU_UNIQUE
} writer_flagprint_bind_e;

typedef enum
{
    WRITER_FLAGPRINT_TYPE_NOTYPE    = ELFPARSER_SYMTABLE_TYPE_NOTYPE,
    WRITER_FLAGPRINT_TYPE_OBLJECT   = ELFPARSER_SYMTABLE_TYPE_OBJECT,
    WRITER_FLAGPRINT_TYPE_FUNC      = ELFPARSER_SYMTABLE_TYPE_FUNC,
    WRITER_FLAGPRINT_TYPE_SECT      = ELFPARSER_SYMTABLE_TYPE_SECT,
    WRITER_FLAGPRINT_TYPE_FILE      = ELFPARSER_SYMTABLE_TYPE_FILE,
    WRITER_FLAGPRINT_TYPE_COMMON    = ELFPARSER_SYMTABLE_TYPE_COMMON,
    WRITER_FLAGPRINT_TYPE_TLS       = ELFPARSER_SYMTABLE_TYPE_TLS,
    WRITER_FLAGPRINT_TYPE_GNU       = ELFPARSER_SYMTABLE_TYPE_GNU_IFUNC
} writer_flagprint_type_e;

#define WRITER_FLAGPRINT_SHIDX_UNDEFINED   0u
#define WRITER_FLAGPRINT_SHIDX_ABSOLUTE    0xfff1u
#define WRITER_FLAGPRINT_SHIDX_COMMON      0xfff2u

void Writer_FlagPrint_sectionHeadLoad(const elfparser_secthead_t *sect_head);
void Writer_FlagPrint_sectionHeadUnload(void);

#endif /* _IG_WRITER_FLAGPRINT_ */