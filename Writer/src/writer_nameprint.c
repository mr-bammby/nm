#include "../inc_priv/writer_valueprint_priv.h"
#include <unistd.h>

#define MAX_LEN 16
#define LEN_TO_PRINT MAX_LEN
#define UNDEF_VALUE "                "
#define ZEROS       "0000000000000000"

int Writer_NamePrint_print(char* name)
{
    unsigned long len = 0;
    int ret_val;

    while(name[len] != '\0')
    {
        len++;
    }
    ret_val = write(STDOUT_FILENO, name, len);
    if (ret_val != -1)
    {
        ret_val = 0;
    }
    return(ret_val);
}
