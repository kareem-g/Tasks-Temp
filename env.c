#include "shell.h"

/**
 * @brief Prints the current environment.
 * @param info Structure containing potential arguments.
 * @return Always 0
 */
int displayEnvironment(info_t *info)
{
    printLinkedListStrings(info->environmentList);
    return 0;
}

/**
 * @brief Gets the value of an environment variable.
 * @param info Structure containing potential arguments.
 * @param name Environment variable name.
 * @return The value of the environment variable.
 */
char *getEnvironmentVariable(info_t *info, const char *name)
{
    list_t *node = info->environmentList;
    char *value;

    while (node)
    {
        value = startsWith(node->str, name);
        if (value && *value)
            return value;
        node = node->next;
    }
    return NULL;
}

/**
 * @brief Initializes a new environment variable or modifies an existing one.
 * @param info Structure containing potential arguments.
 * @return Always 0
 */
int setEnvironmentVariable(info_t *info)
{
    if (info->argumentCount != 3)
    {
        printErrorMessage("Incorrect number of arguments\n");
        return 1;
    }

    if (updateEnvironmentVariable(info, info->arguments[1], info->arguments[2]))
        return 0;

    return 1;
}

/**
 * @brief Removes an environment variable.
 * @param info Structure containing potential arguments.
 * @return Always 0
 */
int unsetEnvironmentVariable(info_t *info)
{
    int i;

    if (info->argumentCount == 1)
    {
        printErrorMessage("Too few arguments.\n");
        return 1;
    }

    for (i = 1; i <= info->argumentCount; i++)
        removeEnvironmentVariable(info, info->arguments[i]);

    return 0;
}

/**
 * @brief Populates the environment linked list.
 * @param info Structure containing potential arguments.
 * @return Always 0
 */
int populateEnvironmentList(info_t *info)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; systemEnvironment[i]; i++)
        addNodeEnd(&node, systemEnvironment[i], 0);

    info->environmentList = node;
    return 0;
}
