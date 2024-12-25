#ifndef _IG_WRITER_FLAGPRINT_PRIV_
#define _IG_WRITER_FLAGPRINT_PRIV_

#include "../inc_pub/writer_flagprint.h"

#define FLAGPRINT_FLAG_ABSOLUTE             "A" /* reserved SHIDX */
#define FLAGPRINT_FLAG_BSS_GLOBAL           "B" /* section name .bss */
#define FLAGPRINT_FLAG_BSS_LOCAL            "b"
#define FLAGPRINT_FLAG_COMMON               "C" /* reserved SHIDX */
#define FLAGPRINT_FLAG_DATA_GLOBAL          "D" /* section name .data and .data1 */
#define FLAGPRINT_FLAG_DATA_LOCAL           "d"
#define FLAGPRINT_FLAG_INDIRECT             "I" /* GNU specific symbol type*/
#define FLAGPRINT_FLAG_GNU_TYPE             "i" /* GNU specific symbol type*/
#define FLAGPRINT_FLAG_DEBUGGING            "N" /* section name .debug */
#define FLAGPRINT_FLAG_RODATA_GLOBAL        "R" /* section name .rodata and .rodata1 */
#define FLAGPRINT_FLAG_RODATA_LOCAL         "r"
#define FLAGPRINT_FLAG_CODE_GLOBAL          "T" /* section name .text */
#define FLAGPRINT_FLAG_CODE_LOCAL           "t"
#define FLAGPRINT_FLAG_UNDEF                "U" /* reserved SHIDX */
#define FLAGPRINT_FLAG_GNU_BIND             "u" /* GNU specific symbol bind*/
#define FLAGPRINT_FLAG_WEAK_OBJECT          "V" /* reserved bind */
#define FLAGPRINT_FLAG_WEAK_OBJECT_UNDEF    "v" 
#define FLAGPRINT_FLAG_WEAK                 "W" /* reserved bind */
#define FLAGPRINT_FLAG_WEAK_UNDEF           "w"
#define FLAGPRINT_FLAG_UNKNOW               "?" /* default */

#define FLAGPRINT_FLAG_LEN 1u

#define FLAGPRINT_SH_NAME_DATA_ARR  ((const char*[]){".data", ".data1", ".dynamic", ".got", ".fini_array", ".init_array"})
#define FLAGPRINT_SH_NAME_DATA_ARR_LEN 6


#define FLAGPRINT_SH_NAME_RODATA_ARR  ((const char*[]){".rodata", ".rodata1", ".note.ABI-tag", ".eh_frame", ".eh_frame_hdr"})
#define FLAGPRINT_SH_NAME_RODATA_ARR_LEN 5

#define FLAGPRINT_SH_NAME_CODE_ARR  ((const char*[]){".text", ".fini", ".init"})
#define FLAGPRINT_SH_NAME_CODE_ARR_LEN 3

#define FLAGPRINT_SH_NAME_BSS ".bss"

#define FLAGPRINT_SH_NAME_DEBUG ".debug"

int Writer_FlagPrint_print(writer_flagprint_bind_e bind, uint16_t symbol_shidx, writer_flagprint_type_e type);

#endif /* _IG_WRITER_FLAGPRINT_PRIV_ */
