#include "shell.h"

/**
 * get_environment_strings - returns the string array copy of our environment variables
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
char **get_environment_strings(info_t *info)
{
    if (!info->environment_strings || info->environment_changed)
    {
        info->environment_strings = list_to_strings(info->environment_list);
        info->environment_changed = 0;
    }

    return (info->environment_strings);
}

/**
 * remove_environment_variable - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: 1 on delete, 0 otherwise
 * @variable: the string representing the environment variable
 */
int remove_environment_variable(info_t *info, char *variable)
{
    list_t *node = info->environment_list;
    size_t index = 0;
    char *position;

    if (!node || !variable)
        return (0);

    while (node)
    {
        position = starts_with(node->str, variable);
        if (position && *position == '=')
        {
            info->environment_changed = delete_node_at_index(&(info->environment_list), index);
            index = 0;
            node = info->environment_list;
            continue;
        }
        node = node->next;
        index++;
    }
    return (info->environment_changed);
}

/**
 * set_environment_variable - Initialize a new environment variable,
 *                            or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: the string representing the environment variable
 * @value: the string representing the environment variable value
 * Return: Always 0
 */
int set_environment_variable(info_t *info, char *variable, char *value)
{
    char *buffer = NULL;
    list_t *node;
    char *position;

    if (!variable || !value)
        return (0);

    buffer = malloc(_strlen(variable) + _strlen(value) + 2);
    if (!buffer)
        return (1);
    _strcpy(buffer, variable);
    _strcat(buffer, "=");
    _strcat(buffer, value);
    node = info->environment_list;
    while (node)
    {
        position = starts_with(node->str, variable);
        if (position && *position == '=')
        {
            free(node->str);
            node->str = buffer;
            info->environment_changed = 1;
            return (0);
        }
        node = node->next;
    }
    add_node_end(&(info->environment_list), buffer, 0);
    free(buffer);
    info->environment_changed = 1;
    return (0);
}
