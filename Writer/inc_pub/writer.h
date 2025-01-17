#ifndef _IG_WRITER_H_
#define _IG_WRITER_H_

#include "writer_flagprint.h"

typedef struct writre_line_s
{
    writer_flagprint_bind_e bind;
    writer_flagprint_type_e type;
    uint16_t sect_head_idx;
    char *name;
    uint64_t value;
} writer_line_t;

int Writer_linePrint(const writer_line_t *line);

#endif /* _IG_WRITER_H_ */
