#include "../inc_pub/writer.h"

#include "../inc_priv/writer_valueprint_priv.h"
#include "../inc_priv/writer_flagprint_priv.h"
#include "../inc_priv/writer_nameprint_priv.h"
#include <unistd.h>


#define SPACE_STR " "
#define SPACE_LEN 1
#define NL_STR     "\n"
#define NL_LEN 1

int Writer_linePrint(const writer_line_t *line)
{
    int ret_val;

    ret_val = Writer_ValuePrint_print(line->value, (line->sect_head_idx == WRITER_FLAGPRINT_SHIDX_UNDEFINED));
    if (ret_val == 0)
    {
        ret_val = write(STDOUT_FILENO, SPACE_STR, SPACE_LEN);
    }
    if (ret_val == 0)
    {
        ret_val = Writer_FlagPrint_print(line->bind, line->sect_head_idx, line->type);
    }
    if (ret_val == 0)
    {
        ret_val = write(STDOUT_FILENO, SPACE_STR, SPACE_LEN);
    }
    if (ret_val == 0)
    {
        ret_val = Writer_NamePrint_print(line->name);
    }  
    if (ret_val == 0)
    {
        ret_val = write(STDOUT_FILENO, SPACE_STR, SPACE_LEN);
    }
    return (ret_val);

    
}
