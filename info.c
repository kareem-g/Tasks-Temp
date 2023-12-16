#include "shell.h"

/**
 * initialize_info - initializes info_t struct
 * @info: struct address
 */
void initialize_info(info_t *info)
{
    info->argument = NULL;
    info->arguments_vector = NULL;
    info->executable_path = NULL;
    info->argument_count = 0;
}

/**
 * populate_info - populates info_t struct with argument vector
 * @info: struct address
 * @argument_vector: argument vector
 */
void populate_info(info_t *info, char **argument_vector)
{
    int i = 0;

    info->filename = argument_vector[0];
    if (info->argument)
    {
        info->arguments_vector = strtow(info->argument, " \t");
        if (!info->arguments_vector)
        {
            info->arguments_vector = malloc(sizeof(char *) * 2);
            if (info->arguments_vector)
            {
                info->arguments_vector[0] = _strdup(info->argument);
                info->arguments_vector[1] = NULL;
            }
        }
        for (i = 0; info->arguments_vector && info->arguments_vector[i]; i++)
            ;
        info->argument_count = i;

        replace_alias(info);
        replace_variables(info);
    }
}

/**
 * release_info - frees allocated memory in info_t struct
 * @info: struct address
 * @free_all: true if freeing all fields
 */
void release_info(info_t *info, int free_all)
{
    ffree(info->arguments_vector);
    info->arguments_vector = NULL;
    info->executable_path = NULL;
    if (free_all)
    {
        if (!info->command_buffer)
            free(info->argument);
        if (info->environment_variables)
            free_list(&(info->environment_variables));
        if (info->command_history)
            free_list(&(info->command_history));
        if (info->aliases)
            free_list(&(info->aliases));
        ffree(info->environment);
        info->environment = NULL;
        bfree((void **)info->command_buffer);
        if (info->read_file_descriptor > 2)
            close(info->read_file_descriptor);
        _put_character(BUFFER_FLUSH);
    }
}
