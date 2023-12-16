#ifndef CUSTOM_SHELL_H
#define CUSTOM_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* Read/write buffer sizes */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

/* Command chaining types */
#define CMD_NORMAL 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* Convert number options */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* Flag for using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE ".custom_shell_history"
#define HISTORY_MAX 4096

extern char **custom_environment;

/**
 * Node structure for a singly linked list
 */
typedef struct StringNode
{
    int num;
    char *str;
    struct StringNode *next;
} StringListNode;

/**
 * Pseudo-arguments structure to pass into a function,
 * allowing a uniform prototype for a function pointer struct
 */
typedef struct ArgumentInfo
{
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int line_count;
    int err_num;
    int linecount_flag;
    char *fname;
    StringListNode *env;
    StringListNode *history;
    StringListNode *alias;
    char **custom_environment;
    int env_changed;
    int status;

    char **cmd_buffer; /* Pointer to command chain buffer, for memory management */
    int cmd_buffer_type; /* Command chaining type (e.g., ||, &&, ;) */
    int read_file_descriptor;
    int history_count;
} ArgumentInfo;

#define ARGUMENT_INFO_INIT \
    {NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0}

/**
 * Structure for a built-in command and its corresponding function
 */
typedef struct BuiltInCommand
{
    char *type;
    int (*function)(ArgumentInfo *);
} BuiltInCommandTable;

/* Function prototypes */
int custom_shell(ArgumentInfo *, char **);
int find_builtin_command(ArgumentInfo *);
void find_command(ArgumentInfo *);
void fork_command(ArgumentInfo *);

/* Other function prototypes and definitions... */

#endif
