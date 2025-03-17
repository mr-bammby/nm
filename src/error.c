/**
 * @file error.c
 * @brief Error handling functions for ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file contains functions for printing various error messages
 * in the ft_nm program, including errors for bad allocations,
 * invalid options, unrecognized file formats, and system errors.
 */

#include <unistd.h>  // For write, STDERR_FILENO
#include <stdio.h>   // For perror

// Application name prefix for error messages
#define APP_NAME "ft_nm: "
// Warning prefix
#define WARNING_NAME "Warning: "

// Error message strings
#define NO_FILE_ERR ": No such file\n"
#define NO_PERMISSION_ERR ": Permission denied\n"
#define IS_DIR_WARN ": is a directory\n"
#define UNKNOWN_FORMAT ": file format not recognized\n"
#define BAD_ALLOC "Malloc failed\n"
#define BAD_OPTION "invalid option -- "

/**
 * @brief Calculates the length of a string
 * @param[in] str Pointer to the string
 * @return unsigned int Length of the string
 */
static unsigned int ft_strlen(const char *str)
{
    unsigned int ret = 0U;  // Initialize return value to 0
    if (str == NULL)               // Check for NULL pointer (bug: condition is incorrect)
    {
        return (ret);       // Return 0 if str is NULL
    }
    while (*str != '\0')    // Loop until end of string
    {
        str++;             // Move to next character
        ret++;             // Increment length counter
    }
    return(ret);           // Return computed length
}

/**
 * @brief Prints the application name to a file descriptor
 * @param[in] fd File descriptor to write to
 */
static void Print_App(int fd)
{
    write(fd, APP_NAME, ft_strlen(APP_NAME));  // Write application name to fd
}

/**
 * @brief Prints a filename enclosed in single quotes to a file descriptor
 * @param[in] fd File descriptor to write to
 * @param[in] filename Name of the file to print
 */
static void Print_File(int fd, const char* filename)
{
    write(fd, "'", 1);              // Write opening single quote
    write(fd, filename, ft_strlen(filename));  // Write filename
    // Note: Missing closing single quote in original code
}

/**
 * @brief Prints an error message for memory allocation failure
 * @return int Always returns 1
 */
int Err_Print_BadAlloc(void)
{
    perror(APP_NAME);  // Print error with app name prefix
    return (1);        // Return error code
}

/**
 * @brief Prints an error message for an invalid command-line option
 * @param[in] option The invalid option character
 * @return int Always returns 1
 */
int Err_Print_BadOption(const char* option)
{
    Print_App(STDERR_FILENO);                          // Print app name to stderr
    write(STDERR_FILENO, BAD_OPTION, ft_strlen(BAD_OPTION));  // Print "invalid option -- "
    write(STDERR_FILENO, "'", 1);                      // Print opening single quote
    write(STDERR_FILENO, option, 1);                   // Print the invalid option char
    write(STDERR_FILENO, "'\n", 2);                    // Print closing quote and newline
    return (1);                                        // Return error code
}

/**
 * @brief Prints an error message for an unrecognized file format
 * @param[in] file_name Name of the file with the bad format
 * @return int Always returns 1
 */
int Err_Print_BadFormat(const char* file_name)
{
    Print_App(STDERR_FILENO);                          // Print app name to stderr
    write(STDERR_FILENO, file_name, ft_strlen(file_name));  // Print filename
    write(STDERR_FILENO, UNKNOWN_FORMAT, ft_strlen(UNKNOWN_FORMAT));  // Print format error
    return (1);                                        // Return error code
}

/**
 * @brief Prints an error message based on the current errno value
 * @param[in] file_name Name of the file associated with the error
 * @return int Always returns 1
 */
int Err_Print_Errno(const char* file_name)
{
    Print_App(STDERR_FILENO);        // Print app name to stderr
    Print_File(STDERR_FILENO, file_name);  // Print quoted filename
    perror("'");                     // Print errno message with quote prefix
    return (1);                      // Return error code
}
