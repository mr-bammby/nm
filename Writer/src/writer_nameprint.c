#include "../inc_priv/writer_valueprint_priv.h"
#include <unistd.h>


#define MAX_LEN 16
#define LEN_TO_PRINT MAX_LEN
#define UNDEF_VALUE "                "
#define ZEROS       "0000000000000000"

int Writer_NamePrint_print(char* name)
{
    unsigned long len = 0;
    while(name[len] != '\0')
    {
        len++;
    }
    return(write(STDOUT_FILENO, name, len));
}