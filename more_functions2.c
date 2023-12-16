#include "shell.h"

/**
 * @brief Converts a string to an integer with error handling
 * 
 * @param str: the string to be converted
 * @return Converted number if successful, -1 on error
 */
int convertStringToIntWithErrHandling(char *str)
{
    int index = 0;
    unsigned long int result = 0;

    if (*str == '+')
        str++;

    for (index = 0; str[index] != '\0'; index++)
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            result *= 10;
            result += (str[index] - '0');
            if (result > INT_MAX)
                return (-1);
        }
        else
            return (-1);
    }
    return (result);
}

/**
 * @brief Prints an error message
 * 
 * @param info: the parameter & return info structure
 * @param errorMsg: string containing specified error type
 */
void printErrorMessage(info_t *info, char *errorMsg)
{
    printFilename(info->fname);
    printCharacter(": ", STDERR_FILENO);
    printLineNumber(info->line_count, STDERR_FILENO);
    printCharacter(": ", STDERR_FILENO);
    printCommand(info->argv[0]);
    printCharacter(": ", STDERR_FILENO);
    printCharacter(errorMsg);
}

/**
 * @brief Prints a decimal (integer) number (base 10)
 * 
 * @param input: the input number
 * @param fd: the file descriptor to write to
 * @return Number of characters printed
 */
int printDecimal(int input, int fd)
{
    int (*customPutchar)(char) = customPutchar;
    int i, count = 0;
    unsigned int absoluteValue, current;

    if (fd == STDERR_FILENO)
        customPutchar = customErrorPutchar;

    if (input < 0)
    {
        absoluteValue = -input;
        customPutchar('-');
        count++;
    }
    else
        absoluteValue = input;

    current = absoluteValue;
    for (i = 1000000000; i > 1; i /= 10)
    {
        if (absoluteValue / i)
        {
            customPutchar('0' + current / i);
            count++;
        }
        current %= i;
    }
    customPutchar('0' + current);
    count++;

    return (count);
}

/**
 * @brief Converts a number to a string representation
 * 
 * @param num: the number to convert
 * @param base: the base for conversion
 * @param flags: argument flags
 * @return String representation of the number
 */
char *convertNumberToString(long int num, int base, int flags)
{
    static char *charArray;
    static char stringBuffer[50];
    char sign = 0;
    char *ptr;
    unsigned long convertedNum = num;

    if (!(flags & CONVERT_UNSIGNED) && num < 0)
    {
        convertedNum = -num;
        sign = '-';
    }

    charArray = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
    ptr = &stringBuffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = charArray[convertedNum % base];
        convertedNum /= base;
    } while (convertedNum != 0);

    if (sign)
        *--ptr = sign;
    return (ptr);
}

/**
 * @brief Replaces the first instance of '#' with '\0'
 * 
 * @param buffer: address of the string to modify
 */
void removeComments(char *buffer)
{
    int index;

    for (index = 0; buffer[index] != '\0'; index++)
        if (buffer[index] == '#' && (!index || buffer[index - 1] == ' '))
        {
            buffer[index] = '\0';
            break;
        }
}
