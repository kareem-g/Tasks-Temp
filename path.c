#include "shell.h"

/**
 * is_executable_command - determines if a file is an executable command
 * @shell_info: the shell information struct
 * @file_path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_executable_command(shell_info_t *shell_info, char *file_path)
{
    struct stat file_stat;

    (void)shell_info;
    if (!file_path || stat(file_path, &file_stat))
        return 0;

    if (file_stat.st_mode & S_IFREG)
    {
        return 1;
    }
    return 0;
}

/**
 * duplicate_characters - duplicates characters
 * @path_string: the PATH string
 * @start_index: starting index
 * @stop_index: stopping index
 *
 * Return: pointer to new buffer
 */
char *duplicate_characters(char *path_string, int start_index, int stop_index)
{
    static char buffer[1024];
    int buffer_index = 0, path_index = 0;

    for (buffer_index = 0, path_index = start_index; path_index < stop_index; path_index++)
        if (path_string[path_index] != ':')
            buffer[buffer_index++] = path_string[path_index];
    buffer[buffer_index] = '\0';
    return buffer;
}

/**
 * find_command_path - finds the command in the PATH string
 * @shell_info: the shell information struct
 * @path_string: the PATH string
 * @command: the command to find
 *
 * Return: full path of the command if found, or NULL
 */
char *find_command_path(shell_info_t *shell_info, char *path_string, char *command)
{
    int path_index = 0, current_position = 0;
    char *full_path;

    if (!path_string)
        return NULL;
    if ((_strlen(command) > 2) && starts_with(command, "./"))
    {
        if (is_executable_command(shell_info, command))
            return command;
    }
    while (1)
    {
        if (!path_string[path_index] || path_string[path_index] == ':')
        {
            full_path = duplicate_characters(path_string, current_position, path_index);
            if (!*full_path)
                _strcat(full_path, command);
            else
            {
                _strcat(full_path, "/");
                _strcat(full_path, command);
            }
            if (is_executable_command(shell_info, full_path))
                return full_path;
            if (!path_string[path_index])
                break;
            current_position = path_index;
        }
        path_index++;
    }
    return NULL;
}
