#ifndef _IG_ERROR_H_
#define _IG_ERROR_H_

int Err_Print_BadAlloc(void);
int Err_Print_BadOption(const char* option);
int Err_Print_NoFile(const char* file_name);
int Err_Print_NoPermission(const char* file_name);
int Err_Print_BadFormat(const char* file_name);
int Err_Print_IsDir(const char* file_name);
int Err_Print_Errno(const char* file_name);

#endif /* _IG_ERROR_H_ */
