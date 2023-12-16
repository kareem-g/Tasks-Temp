#include "shell.h"

/**
 * custom_fill_memory - fills memory with a constant byte
 * @memory_ptr: the pointer to the memory area
 * @byte_value: the byte to fill *memory_ptr with
 * @num_bytes: the number of bytes to be filled
 * Return: a pointer to the memory area memory_ptr
 */
char *custom_fill_memory(char *memory_ptr, char byte_value, unsigned int num_bytes)
{
	unsigned int i;

	for (i = 0; i < num_bytes; i++)
		memory_ptr[i] = byte_value;
	return memory_ptr;
}

/**
 * custom_free_strings - frees a string of strings
 * @string_array: string of strings to be freed
 */
void custom_free_strings(char **string_array)
{
	char **current_ptr = string_array;

	if (!string_array)
		return;
	while (*string_array)
		free(*string_array++);
	free(current_ptr);
}

/**
 * custom_reallocate_memory - reallocates a block of memory
 * @previous_ptr: pointer to the previously allocated block
 * @old_size_bytes: byte size of the previous block
 * @new_size_bytes: byte size of the new block
 * Return: pointer to the reallocated block
 */
void *custom_reallocate_memory(void *previous_ptr, unsigned int old_size_bytes, unsigned int new_size_bytes)
{
	char *new_ptr;

	if (!previous_ptr)
		return malloc(new_size_bytes);
	if (!new_size_bytes)
		return free(previous_ptr), NULL;
	if (new_size_bytes == old_size_bytes)
		return previous_ptr;

	new_ptr = malloc(new_size_bytes);
	if (!new_ptr)
		return NULL;

	old_size_bytes = old_size_bytes < new_size_bytes ? old_size_bytes : new_size_bytes;
	while (old_size_bytes--)
		new_ptr[old_size_bytes] = ((char *)previous_ptr)[old_size_bytes];
	free(previous_ptr);
	return new_ptr;
}
