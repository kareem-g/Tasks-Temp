#include "custom_shell.h"

/**
 * copy_string - copies a string
 * @destination: the destination
 * @source: the source
 *
 * Return: pointer to destination
 */
char *copy_string(char *destination, char *source)
{
	int index = 0;

	if (destination == source || source == NULL)
		return (destination);
	while (source[index])
	{
		destination[index] = source[index];
		index++;
	}
	destination[index] = '\0';
	return (destination);
}

/**
 * duplicate_string - duplicates a string
 * @original: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *duplicate_string(const char *original)
{
	int length = 0;
	char *result;

	if (original == NULL)
		return (NULL);
	while (*original++)
		length++;
	result = malloc(sizeof(char) * (length + 1));
	if (!result)
		return (NULL);
	for (length++; length--;)
		result[length] = *--original;
	return (result);
}

/**
 * print_string - prints an input string
 * @input_str: the string to be printed
 *
 * Return: Nothing
 */
void print_string(char *input_str)
{
	int index = 0;

	if (!input_str)
		return;
	while (input_str[index] != '\0')
	{
		put_character(input_str[index]);
		index++;
	}
}

/**
 * put_character - writes the character to stdout
 * @character: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int put_character(char character)
{
	static int i;
	static char buffer[WRITE_BUFFER_SIZE];

	if (character == FLUSH_BUFFER || i >= WRITE_BUFFER_SIZE)
	{
		write(1, buffer, i);
		i = 0;
	}
	if (character != FLUSH_BUFFER)
		buffer[i++] = character;
	return (1);
}
