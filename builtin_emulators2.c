#include "shell.h"

/**
 * displayCommandHistory - Displays the history list, one command per line,
 *                         preceded with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int displayCommandHistory(info_t *shellInfo)
{
    printList(shellInfo->commandHistory);
    return 0;
}

/**
 * removeAlias - Removes an alias by setting it to an empty string.
 * @info: Parameter struct
 * @aliasString: The string alias
 * Return: Always 0 on success, 1 on error
 */
int removeAlias(info_t *shellInfo, char *aliasString)
{
    char *equalSign, originalCharacter;
    int result;

    equalSign = findCharacter(aliasString, '=');
    if (!equalSign)
        return 1;

    originalCharacter = *equalSign;
    *equalSign = '\0';

    result = deleteNodeAtIndex(&(shellInfo->aliases),
        getNodeIndex(shellInfo->aliases, nodeStartsWith(shellInfo->aliases, aliasString, -1)));

    *equalSign = originalCharacter;
    return result;
}

/**
 * setAlias - Sets an alias to a specified string.
 * @info: Parameter struct
 * @aliasString: The string alias
 * Return: Always 0 on success, 1 on error
 */
int setAlias(info_t *shellInfo, char *aliasString)
{
    char *equalSign;

    equalSign = findCharacter(aliasString, '=');
    if (!equalSign)
        return 1;

    if (!*++equalSign)
        return removeAlias(shellInfo, aliasString);

    removeAlias(shellInfo, aliasString);
    return (addNodeEnd(&(shellInfo->aliases), aliasString, 0) == NULL);
}

/**
 * printAliasString - Prints an alias string.
 * @aliasNode: The alias node
 * Return: Always 0 on success, 1 on error
 */
int printAliasString(list_t *aliasNode)
{
    char *equalSign = NULL, *aliasSubstring = NULL;

    if (aliasNode)
    {
        equalSign = findCharacter(aliasNode->str, '=');
        for (aliasSubstring = aliasNode->str; aliasSubstring <= equalSign; aliasSubstring++)
            _putchar(*aliasSubstring);
        _putchar('\'');
        _puts(equalSign + 1);
        _puts("'\n");
        return 0;
    }
    return 1;
}

/**
 * manageAliases - Mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int manageAliases(info_t *shellInfo)
{
    int i = 0;
    char *equalSign = NULL;
    list_t *aliasNode = NULL;

    if (shellInfo->argc == 1)
    {
        aliasNode = shellInfo->aliases;
        while (aliasNode)
        {
            printAliasString(aliasNode);
            aliasNode = aliasNode->next;
        }
        return 0;
    }

    for (i = 1; shellInfo->argv[i]; i++)
    {
        equalSign = findCharacter(shellInfo->argv[i], '=');
        if (equalSign)
            setAlias(shellInfo, shellInfo->argv[i]);
        else
            printAliasString(nodeStartsWith(shellInfo->aliases, shellInfo->argv[i], '='));
    }

    return 0;
}
