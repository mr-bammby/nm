#include "../inc_priv/writer_valueprint_priv.h"
#include <unistd.h>

#include <stdio.h>


#define MAX_LEN 16
#define LEN_TO_PRINT MAX_LEN
#define UNDEF_VALUE "                "
#define ZEROS       "0000000000000000"

static int8_t valueprint_ulltoa_hex(uint64_t num, char** ret_str)
{
    char base[] = "0123456789abcdef";
    uint8_t base_size = 16;
    uint64_t factor = 1;
    int8_t num_len = 0;
    short max_len = MAX_LEN;
    (*ret_str)[max_len] = '\0';
    uint8_t next_num;

    do{
        next_num = num % 16;
        num -= (next_num);
        num /= 16;
        factor *= base_size;
        max_len--;
        num_len++;
        (*ret_str)[max_len] = base[next_num];
    }
    while ((max_len != 0) && (num != 0));
    *ret_str = &((*ret_str)[max_len]);
    return(num_len);
}

int Writer_ValuePrint_print(uint16_t value, uint8_t is_undefined)
{
    char num_val[MAX_LEN + 1];
    char *num_val_p = num_val;
    int8_t num_len;
    int temp;
    
    if (is_undefined)
    {
        temp = write(STDOUT_FILENO, UNDEF_VALUE, LEN_TO_PRINT);
        if (temp == -1)
        {
            return(temp);
        }
        return(0);
    }
    num_len = valueprint_ulltoa_hex(value, &num_val_p);
    temp = write(STDOUT_FILENO, ZEROS, LEN_TO_PRINT - num_len);
    if (temp == -1)
    {
        return(temp);
    }
    temp = write(STDOUT_FILENO, num_val_p, num_len);
    if (temp == -1)
    {
        return(temp);
    }
    return(0);
}