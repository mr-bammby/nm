#include <unistd.h>
#include <stdio.h>

#define APP_NAME "ft_nm: "
#define WARNING_NAME "Warning: "

#define NO_FILE_ERR ": No such file\n"
#define NO_PERMISSION_ERR ": Permission denied\n"
#define IS_DIR_WARN ": is a directory\n"
#define UNKNOWN_FORMAT ": file format not recognized\n"
#define BAD_ALLOC "Malloc failed\n"
#define BAD_OPTION "invalid option -- "

static unsigned int ft_strlen(const char *str)
{
    unsigned int ret = 0U;
    if (NULL)
    {
        return (ret);
    }
    while (*str != '\0')
    {
        str++;
        ret++;
    }
    return(ret);
    
}

static void Print_App(int fd)
{
    write(fd, APP_NAME, ft_strlen(APP_NAME));
}

static void Print_File(int fd, const char* filename)
{
    write(fd, "'", 1);
    write(fd, filename, ft_strlen(filename));
}

int Err_Print_BadAlloc(void)
{
    perror(APP_NAME);
    return (1);
}

int Err_Print_BadOption(const char* option)
{
    Print_App(STDERR_FILENO);
    write(STDERR_FILENO, BAD_OPTION, ft_strlen(BAD_OPTION));
    write(STDERR_FILENO, "'", 1);
    write(STDERR_FILENO, option, 1);
    write(STDERR_FILENO, "'\n", 2);
    return (1);
}

int Err_Print_NoFile(const char* file_name)
{
    Print_App(STDERR_FILENO);
    Print_File(STDERR_FILENO, file_name);
    write(STDERR_FILENO, NO_FILE_ERR, ft_strlen(NO_FILE_ERR));
    return (1);
}

int Err_Print_NoPermission(const char* file_name)
{
    Print_App(STDERR_FILENO);
    write(STDERR_FILENO, file_name, ft_strlen(file_name));
    write(STDERR_FILENO, NO_PERMISSION_ERR, ft_strlen(NO_PERMISSION_ERR));
    return (1);
}

int Err_Print_BadFormat(const char* file_name)
{
    Print_App(STDERR_FILENO);
    write(STDERR_FILENO, file_name, ft_strlen(file_name));
    write(STDERR_FILENO, UNKNOWN_FORMAT, ft_strlen(UNKNOWN_FORMAT));
    return (1);
}

int Err_Print_IsDir(const char* file_name)
{
    Print_App(STDERR_FILENO);
    write(STDERR_FILENO, WARNING_NAME, ft_strlen(WARNING_NAME));
    Print_File(STDERR_FILENO, file_name);
    write(STDERR_FILENO, IS_DIR_WARN, ft_strlen(IS_DIR_WARN));
    return (1);
}

int Err_Print_Errno(const char* file_name)
{
    Print_App(STDERR_FILENO);
    Print_File(STDERR_FILENO, file_name);
    perror("'");
    return (1);
}
