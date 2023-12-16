#include "shell.h"

/**
 * custom_str_copy - copies a string with a specified length
 * @destination: the destination string to be copied to
 * @source: the source string
 * @max_length: the maximum number of characters to be copied
 * Return: a pointer to the destination string
 */
char *custom_str_copy(char *destination, char *source, int max_length)
{
    int source_index, destination_index;
    char *result = destination;

    source_index = 0;
    while (source[source_index] != '\0' && source_index < max_length - 1)
    {
        destination[source_index] = source[source_index];
        source_index++;
    }

    if (source_index < max_length)
    {
        destination_index = source_index;
        while (destination_index < max_length)
        {
            destination[destination_index] = '\0';
            destination_index++;
        }
    }

    return result;
}

/**
 * custom_str_concat - concatenates two strings with a specified limit
 * @first_str: the first string
 * @second_str: the second string
 * @max_length: the maximum number of bytes to be used
 * Return: a pointer to the concatenated string
 */
char *custom_str_concat(char *first_str, char *second_str, int max_length)
{
    int first_index, second_index;
    char *result = first_str;

    first_index = 0;
    while (first_str[first_index] != '\0')
        first_index++;

    second_index = 0;
    while (second_str[second_index] != '\0' && second_index < max_length)
    {
        first_str[first_index] = second_str[second_index];
        first_index++;
        second_index++;
    }

    if (second_index < max_length)
        first_str[first_index] = '\0';

    return result;
}

/**
 * custom_str_find - locates a character in a string
 * @str: the string to be parsed
 * @character: the character to look for
 * Return: a pointer to the first occurrence of the character in the string, or NULL if not found
 */
char *custom_str_find(char *str, char character)
{
    do
    {
        if (*str == character)
            return str;
    } while (*str++ != '\0');

    return NULL;
}
