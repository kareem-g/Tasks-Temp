#include "custom_shell.h"

/**
 * generate_history_filepath - generates the history file path
 * @shell_info: parameter struct for shell information
 *
 * Return: allocated string containing history file path
 */
char *generate_history_filepath(shell_info_t *shell_info)
{
    char *directory, *filepath;

    directory = get_environment_variable(shell_info, "HOME=");
    if (!directory)
        return NULL;

    filepath = malloc(sizeof(char) * (string_length(directory) + string_length(HISTORY_FILE) + 2));
    if (!filepath)
        return NULL;

    filepath[0] = 0;
    copy_string(filepath, directory);
    concatenate_string(filepath, "/");
    concatenate_string(filepath, HISTORY_FILE);

    return filepath;
}

/**
 * save_history - creates or appends to a history file
 * @shell_info: parameter struct for shell information
 *
 * Return: 1 on success, else -1
 */
int save_history(shell_info_t *shell_info)
{
    ssize_t file_descriptor;
    char *filename = generate_history_filepath(shell_info);
    history_node_t *current_node = NULL;

    if (!filename)
        return -1;

    file_descriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);

    if (file_descriptor == -1)
        return -1;

    for (current_node = shell_info->command_history; current_node; current_node = current_node->next)
    {
        write_string_to_file_descriptor(current_node->command, file_descriptor);
        put_char_to_file_descriptor('\n', file_descriptor);
    }

    put_char_to_file_descriptor(BUFFER_FLUSH, file_descriptor);
    close(file_descriptor);

    return 1;
}

/**
 * load_history - reads history from a file
 * @shell_info: parameter struct for shell information
 *
 * Return: history count on success, 0 otherwise
 */
int load_history(shell_info_t *shell_info)
{
    int i, last = 0, line_count = 0;
    ssize_t file_descriptor, read_length, file_size = 0;
    struct stat file_stat;
    char *buffer = NULL, *filename = generate_history_filepath(shell_info);

    if (!filename)
        return 0;

    file_descriptor = open(filename, O_RDONLY);
    free(filename);

    if (file_descriptor == -1)
        return 0;

    if (!fstat(file_descriptor, &file_stat))
        file_size = file_stat.st_size;

    if (file_size < 2)
        return 0;

    buffer = malloc(sizeof(char) * (file_size + 1));
    if (!buffer)
        return 0;

    read_length = read(file_descriptor, buffer, file_size);
    buffer[file_size] = 0;

    if (read_length <= 0)
        return free(buffer), 0;

    close(file_descriptor);

    for (i = 0; i < file_size; i++)
        if (buffer[i] == '\n')
        {
            buffer[i] = 0;
            append_to_history_list(shell_info, buffer + last, line_count++);
            last = i + 1;
        }

    if (last != i)
        append_to_history_list(shell_info, buffer + last, line_count++);

    free(buffer);
    shell_info->history_count = line_count;

    while (shell_info->history_count-- >= HISTORY_MAX)
        delete_node_at_index(&(shell_info->command_history), 0);

    renumber_history(shell_info);
    return shell_info->history_count;
}

/**
 * append_to_history_list - adds entry to a history linked list
 * @shell_info: parameter struct for shell information
 * @buffer: buffer containing the command
 * @line_count: the history line count
 *
 * Return: Always 0
 */
int append_to_history_list(shell_info_t *shell_info, char *buffer, int line_count)
{
    history_node_t *current_node = NULL;

    if (shell_info->command_history)
        current_node = shell_info->command_history;

    add_node_end(&current_node, buffer, line_count);

    if (!shell_info->command_history)
        shell_info->command_history = current_node;

    return 0;
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @shell_info: parameter struct for shell information
 *
 * Return: the new history count
 */
int renumber_history(shell_info_t *shell_info)
{
    history_node_t *current_node = shell_info->command_history;
    int i = 0;

    while (current_node)
    {
        current_node->number = i++;
        current_node = current_node->next;
    }

    return shell_info->history_count = i;
}
