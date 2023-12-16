#include "shell.h"

/**
 * print_error_string - prints an input string to stderr
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void print_error_string(char *str)
{
    int index = 0;

    if (!str)
        return;

    while (str[index] != '\0')
    {
        write_to_error(str[index]);
        index++;
    }
}

/**
 * write_to_error - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_to_error(char c)
{
    static int buffer_index;
    static char buffer[WRITE_BUFFER_SIZE];

    if (c == FLUSH_BUFFER || buffer_index >= WRITE_BUFFER_SIZE)
    {
        write(2, buffer, buffer_index);
        buffer_index = 0;
    }

    if (c != FLUSH_BUFFER)
        buffer[buffer_index++] = c;

    return 1;
}

/**
 * write_to_fd - writes the character c to a given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_to_fd(char c, int fd)
{
    static int buffer_index;
    static char buffer[WRITE_BUFFER_SIZE];

    if (c == FLUSH_BUFFER || buffer_index >= WRITE_BUFFER_SIZE)
    {
        write(fd, buffer, buffer_index);
        buffer_index = 0;
    }

    if (c != FLUSH_BUFFER)
        buffer[buffer_index++] = c;

    return 1;
}

/**
 * print_to_fd - prints an input string to a given file descriptor
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters written
 */
int print_to_fd(char *str, int fd)
{
    int count = 0;

    if (!str)
        return 0;

    while (*str)
    {
        count += write_to_fd(*str++, fd);
    }

    return count;
}
