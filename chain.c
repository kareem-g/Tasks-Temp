#include "shell.h"

/**
 * is_chain_delimiter - test if the current character in the buffer is a chain delimiter
 * @info: the parameter struct
 * @buffer: the character buffer
 * @position: address of the current position in buffer
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int is_chain_delimiter(info_t *info, char *buffer, size_t *position)
{
    size_t j = *position;

    if (buffer[j] == '|' && buffer[j + 1] == '|')
    {
        buffer[j] = 0;
        j++;
        info->cmd_buf_type = CMD_OR;
    }
    else if (buffer[j] == '&' && buffer[j + 1] == '&')
    {
        buffer[j] = 0;
        j++;
        info->cmd_buf_type = CMD_AND;
    }
    else if (buffer[j] == ';') /* found end of this command */
    {
        buffer[j] = 0; /* replace semicolon with null */
        info->cmd_buf_type = CMD_CHAIN;
    }
    else
        return (0);
    *position = j;
    return (1);
}

/**
 * check_chain_status - checks if we should continue chaining based on the last status
 * @info: the parameter struct
 * @buffer: the character buffer
 * @position: address of the current position in buffer
 * @start_position: starting position in buffer
 * @length: length of buffer
 *
 * Return: Void
 */
void check_chain_status(info_t *info, char *buffer, size_t *position, size_t start_position, size_t length)
{
    size_t j = *position;

    if (info->cmd_buf_type == CMD_AND)
    {
        if (info->status)
        {
            buffer[start_position] = 0;
            j = length;
        }
    }
    if (info->cmd_buf_type == CMD_OR)
    {
        if (!info->status)
        {
            buffer[start_position] = 0;
            j = length;
        }
    }

    *position = j;
}

/**
 * substitute_alias - substitutes an alias in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int substitute_alias(info_t *info)
{
    int i;
    list_t *node;
    char *p;

    for (i = 0; i < 10; i++)
    {
        node = node_starts_with(info->alias, info->argv[0], '=');
        if (!node)
            return (0);
        free(info->argv[0]);
        p = _strchr(node->str, '=');
        if (!p)
            return (0);
        p = _strdup(p + 1);
        if (!p)
            return (0);
        info->argv[0] = p;
    }
    return (1);
}

/**
 * substitute_variables - substitutes variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int substitute_variables(info_t *info)
{
    int i = 0;
    list_t *node;

    for (i = 0; info->argv[i]; i++)
    {
        if (info->argv[i][0] != '$' || !info->argv[i][1])
            continue;

        if (!_strcmp(info->argv[i], "$?"))
        {
            update_string(&(info->argv[i]), _strdup(convert_number(info->status, 10, 0)));
            continue;
        }
        if (!_strcmp(info->argv[i], "$$"))
        {
            update_string(&(info->argv[i]), _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(info->env, &info->argv[i][1], '=');
        if (node)
        {
            update_string(&(info->argv[i]), _strdup(_strchr(node->str, '=') + 1));
            continue;
        }
        update_string(&info->argv[i], _strdup(""));
    }
    return (0);
}

/**
 * update_string - updates the string
 * @original: address of the old string
 * @replacement: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int update_string(char **original, char *replacement)
{
    free(*original);
    *original = replacement;
    return (1);
}
