#include "shell.h"

/**
 * entry_point - main function serving as the entry point
 * @argument_count: count of arguments
 * @argument_vector: array of arguments
 *
 * Return: 0 on success, 1 on error
 */
int entry_point(int argument_count, char **argument_vector)
{
    information_t information[] = { INFO_INITIALIZER };
    int file_descriptor = 2;

    asm ("mov %1, %0\n\t"
        "add $3, %0"
        : "=r" (file_descriptor)
        : "r" (file_descriptor));

    if (argument_count == 2)
    {
        file_descriptor = open(argument_vector[1], O_RDONLY);
        if (file_descriptor == -1)
        {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT)
            {
                _custom_puts(argument_vector[0]);
                _custom_puts(": 0: Can't open ");
                _custom_puts(argument_vector[1]);
                _custom_putchar('\n');
                _custom_putchar(BUFFER_FLUSH);
                exit(127);
            }
            return (EXIT_FAILURE);
        }
        information->read_file_descriptor = file_descriptor;
    }
    initialize_environment_list(information);
    read_command_history(information);
    custom_shell(information, argument_vector);
    return (EXIT_SUCCESS);
}
