#include "shell.h"

/**
 * add_list_node - adds a node to the start of the list
 * @list_head: address of pointer to head node
 * @data: data field of node
 * @index: node index used by history
 *
 * Return: size of list
 */
list_t *add_list_node(list_t **list_head, const char *data, int index)
{
	list_t *new_node;

	if (!list_head)
		return (NULL);

	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	_initialize_list_node(new_node);
	new_node->index = index;

	if (data)
	{
		new_node->data = _strdup(data);
		if (!new_node->data)
		{
			free(new_node);
			return (NULL);
		}
	}

	new_node->next = *list_head;
	*list_head = new_node;
	return (new_node);
}

/**
 * add_list_node_end - adds a node to the end of the list
 * @list_head: address of pointer to head node
 * @data: data field of node
 * @index: node index used by history
 *
 * Return: size of list
 */
list_t *add_list_node_end(list_t **list_head, const char *data, int index)
{
	list_t *new_node, *current_node;

	if (!list_head)
		return (NULL);

	current_node = *list_head;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	_initialize_list_node(new_node);
	new_node->index = index;

	if (data)
	{
		new_node->data = _strdup(data);
		if (!new_node->data)
		{
			free(new_node);
			return (NULL);
		}
	}

	if (current_node)
	{
		while (current_node->next)
			current_node = current_node->next;
		current_node->next = new_node;
	}
	else
		*list_head = new_node;

	return (new_node);
}

/**
 * print_list_data - prints only the data element of a list_t linked list
 * @head: pointer to first node
 *
 * Return: size of list
 */
size_t print_list_data(const list_t *head)
{
	size_t size = 0;

	while (head)
	{
		_print_data(head->data ? head->data : "(nil)");
		_print_data("\n");
		head = head->next;
		size++;
	}

	return (size);
}

/**
 * remove_node_at_index - removes node at given index
 * @list_head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int remove_node_at_index(list_t **list_head, unsigned int index)
{
	list_t *current_node, *previous_node;
	unsigned int i = 0;

	if (!list_head || !*list_head)
		return (0);

	if (!index)
	{
		current_node = *list_head;
		*list_head = (*list_head)->next;
		_free_list_node(current_node);
		return (1);
	}

	current_node = *list_head;
	while (current_node)
	{
		if (i == index)
		{
			previous_node->next = current_node->next;
			_free_list_node(current_node);
			return (1);
		}
		i++;
		previous_node = current_node;
		current_node = current_node->next;
	}

	return (0);
}

/**
 * clear_list - frees all nodes of a list
 * @list_head_ptr: address of pointer to head node
 *
 * Return: void
 */
void clear_list(list_t **list_head_ptr)
{
	list_t *current_node, *next_node, *head;

	if (!list_head_ptr || !*list_head_ptr)
		return;

	head = *list_head_ptr;
	current_node = head;

	while (current_node)
	{
		next_node = current_node->next;
		_free_list_node(current_node);
		current_node = next_node;
	}

	*list_head_ptr = NULL;
}

/* Helper Functions */

/**
 * _initialize_list_node - initializes a new list node
 * @node: pointer to the node
 *
 * Return: void
 */
void _initialize_list_node(list_t *node)
{
	if (node)
	{
		_memset((void *)node, 0, sizeof(list_t));
	}
}

/**
 * _free_list_node - frees memory occupied by a list node
 * @node: pointer to the node
 *
 * Return: void
 */
void _free_list_node(list_t *node)
{
	if (node)
	{
		free(node->data);
		free(node);
	}
}

/**
 * _print_data - prints data with newline
 * @data: data to be printed
 *
 * Return: void
 */
void _print_data(const char *data)
{
	_puts(data);
	_puts("\n");
}
