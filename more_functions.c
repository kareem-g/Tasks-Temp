#include "shell.h"

/**
 * is_interactive_mode - returns true if the shell is in interactive mode
 * @shell_info: struct containing shell information
 *
 * Return: 1 if in interactive mode, 0 otherwise
 */
int is_interactive_mode(ShellInfo *shell_info)
{
	return (isatty(STDIN_FILENO) && shell_info->input_fd <= 2);
}

/**
 * is_delimiter - checks if a character is a delimiter
 * @character: the character to check
 * @delimiter_str: the delimiter string
 * Return: 1 if true, 0 if false
 */
int is_delimiter(char character, char *delimiter_str)
{
	while (*delimiter_str)
		if (*delimiter_str++ == character)
			return (1);
	return (0);
}

/**
 * is_alphabetic - checks for an alphabetic character
 * @character: The character to check
 * Return: 1 if character is alphabetic, 0 otherwise
 */
int is_alphabetic(int character)
{
	if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * convert_to_integer - converts a string to an integer
 * @string: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */
int convert_to_integer(char *string)
{
	int index, sign = 1, flag = 0, result;
	unsigned int converted_value = 0;

	for (index = 0;  string[index] != '\0' && flag != 2; index++)
	{
		if (string[index] == '-')
			sign *= -1;

		if (string[index] >= '0' && string[index] <= '9')
		{
			flag = 1;
			converted_value *= 10;
			converted_value += (string[index] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		result = -converted_value;
	else
		result = converted_value;

	return (result);
}
