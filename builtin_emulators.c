#include "shell.h"

/**
 * @brief Exit the shell with a given exit status.
 *
 * @param shellInfo Structure containing potential arguments.
 * @return 0 if info->argv[0] is not "exit", -2 if successful exit, 1 on error.
 */
int shellExit(shell_info_t *shellInfo)
{
    int exitCode;

    if (shellInfo->arguments[1])
    {
        exitCode = parseExitCode(shellInfo->arguments[1]);
        if (exitCode == -1)
        {
            shellInfo->status = 2;
            printError(shellInfo, "Illegal number: ");
            printErrorMessage(shellInfo->arguments[1]);
            _eputchar('\n');
            return 1;
        }
        shellInfo->errorNumber = exitCode;
        return -2;
    }
    shellInfo->errorNumber = -1;
    return -2;
}

/**
 * @brief Change the current directory of the process.
 *
 * @param shellInfo Structure containing potential arguments.
 * @return Always 0.
 */
int changeDirectory(shell_info_t *shellInfo)
{
    char *currentPath, *newDirectory, buffer[1024];
    int chdirResult;

    currentPath = getcwd(buffer, 1024);
    if (!currentPath)
        _puts("TODO: >>getcwd failure emsg here<<\n");
    if (!shellInfo->arguments[1])
    {
        newDirectory = _getenv(shellInfo, "HOME=");
        if (!newDirectory)
            chdirResult = chdir((newDirectory = _getenv(shellInfo, "PWD=")) ? newDirectory : "/");
        else
            chdirResult = chdir(newDirectory);
    }
    else if (_strcmp(shellInfo->arguments[1], "-") == 0)
    {
        if (!_getenv(shellInfo, "OLDPWD="))
        {
            _puts(currentPath);
            _putchar('\n');
            return 1;
        }
        _puts(_getenv(shellInfo, "OLDPWD=")), _putchar('\n');
        chdirResult = chdir((newDirectory = _getenv(shellInfo, "OLDPWD=")) ? newDirectory : "/");
    }
    else
        chdirResult = chdir(shellInfo->arguments[1]);
    if (chdirResult == -1)
    {
        printError(shellInfo, "can't cd to ");
        printErrorMessage(shellInfo->arguments[1]);
        _eputchar('\n');
    }
    else
    {
        _setenv(shellInfo, "OLDPWD", _getenv(shellInfo, "PWD="));
        _setenv(shellInfo, "PWD", getcwd(buffer, 1024));
    }
    return 0;
}

/**
 * @brief Display help information.
 *
 * @param shellInfo Structure containing potential arguments.
 * @return Always 0.
 */
int showHelp(shell_info_t *shellInfo)
{
    char **argumentArray;

    argumentArray = shellInfo->arguments;
    _puts("help call works. Function not yet implemented \n");
    if (0)
        _puts(*argumentArray); /* temp att_unused workaround */
    return 0;
}

