#include "shell.h"

/**
 * count_nodes - determines the number of nodes in a linked list
 * @head: pointer to the first node
 *
 * Return: number of nodes in the list
 */
size_t count_nodes(const Node *head)
{
    size_t node_count = 0;

    while (head)
    {
        head = head->next;
        node_count++;
    }
    return node_count;
}

/**
 * convert_list_to_strings - returns an array of strings from the list's str field
 * @list_head: pointer to the first node
 *
 * Return: array of strings or NULL if list is empty
 */
char **convert_list_to_strings(Node *list_head)
{
    Node *current_node = list_head;
    size_t list_size = count_nodes(list_head);
    char **strings_array;
    char *str;

    if (!list_head || !list_size)
        return NULL;

    strings_array = malloc(sizeof(char *) * (list_size + 1));
    if (!strings_array)
        return NULL;

    for (size_t i = 0; current_node; current_node = current_node->next, i++)
    {
        str = malloc(_string_length(current_node->str) + 1);
        if (!str)
        {
            for (size_t j = 0; j < i; j++)
                free(strings_array[j]);
            free(strings_array);
            return NULL;
        }

        str = _copy_string(str, current_node->str);
        strings_array[i] = str;
    }

    strings_array[list_size] = NULL;
    return strings_array;
}

/**
 * display_list - prints all elements of a Node linked list
 * @list_head: pointer to the first node
 *
 * Return: size of the list
 */
size_t display_list(const Node *list_head)
{
    size_t node_count = 0;

    while (list_head)
    {
        _put_string(convert_number(list_head->num, 10, 0));
        _put_character(':');
        _put_character(' ');
        _put_string(list_head->str ? list_head->str : "(nil)");
        _put_string("\n");
        list_head = list_head->next;
        node_count++;
    }
    return node_count;
}

/**
 * find_node_with_prefix - returns the node whose string starts with a given prefix
 * @list_node: pointer to the list head
 * @prefix: string to match
 * @next_char: the next character after the prefix to match
 *
 * Return: matching node or NULL if not found
 */
Node *find_node_with_prefix(Node *list_node, char *prefix, char next_char)
{
    char *prefix_match = NULL;

    while (list_node)
    {
        prefix_match = starts_with(list_node->str, prefix);
        if (prefix_match && ((next_char == -1) || (*prefix_match == next_char)))
            return list_node;
        list_node = list_node->next;
    }
    return NULL;
}

/**
 * get_node_index_in_list - gets the index of a node in the list
 * @list_head: pointer to the list head
 * @target_node: pointer to the target node
 *
 * Return: index of the target node or -1 if not found
 */
ssize_t get_node_index_in_list(Node *list_head, Node *target_node)
{
    size_t index = 0;

    while (list_head)
    {
        if (list_head == target_node)
            return index;
        list_head = list_head->next;
        index++;
    }
    return -1;
}
