#include "../inc_priv/writer_valueprint_priv.h"
#include <unistd.h>

#define MAX_LEN_64 16
#define LEN_TO_PRINT_64 MAX_LEN_64
#define UNDEF_VALUE_64 "                "
#define ZEROS_64       "0000000000000000"

#define MAX_LEN_32 8
#define LEN_TO_PRINT_32 MAX_LEN_32
#define UNDEF_VALUE_32 "        "
#define ZEROS_32       "00000000"



static int8_t valueprint_ulltoa_hex(uint64_t num, char** ret_str, writer_bit_t bit_len)
{
    char base[] = "0123456789abcdef";
    uint8_t base_size = 16;
    uint64_t factor = 1;
    int8_t num_len = 0;
    uint8_t next_num;
    short max_len = (bit_len == WRITER_VALUEPRINT_32BIT) ? (MAX_LEN_32) : (MAX_LEN_64);

    // Initialize the string with zeros
    for(int i = 0; i < max_len; i++) {
        (*ret_str)[i] = '0';
    }
    (*ret_str)[max_len] = '\0';
    
    do{
        next_num = num % base_size;
        num -= (next_num);
        num /= base_size;
        factor *= base_size;
        max_len--;
        num_len++;
        (*ret_str)[max_len] = base[next_num];
    }
    while ((max_len != 0) && (num != 0));
    *ret_str = &((*ret_str)[max_len]);
    return(num_len);
}

int Writer_ValuePrint_print(uint64_t value, uint8_t is_undefined, writer_bit_t bit_len)
{
    char num_val[((bit_len == WRITER_VALUEPRINT_32BIT) ? (MAX_LEN_32) : (MAX_LEN_64)) + 1];
    char *num_val_p = num_val;
    int8_t num_len;
    int temp;
    
    if (is_undefined)
    {
        temp = write(STDOUT_FILENO, ((bit_len == WRITER_VALUEPRINT_32BIT) ? (UNDEF_VALUE_32) : (UNDEF_VALUE_64)), ((bit_len == WRITER_VALUEPRINT_32BIT) ? (LEN_TO_PRINT_32) : (LEN_TO_PRINT_64)));
        if (temp == -1)
        {
            return(temp);
        }
        return(0);
    }
    num_len = valueprint_ulltoa_hex(value, &num_val_p, bit_len);
    temp = write(STDOUT_FILENO, ((bit_len == WRITER_VALUEPRINT_32BIT) ? (ZEROS_32) : (ZEROS_64)), ((bit_len == WRITER_VALUEPRINT_32BIT) ? (LEN_TO_PRINT_32) : (LEN_TO_PRINT_64)) - num_len);
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
