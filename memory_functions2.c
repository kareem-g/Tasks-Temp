#include "shell.h"

/**
 * releaseMemory - frees a pointer and NULLs the address
 * @memoryPtr: address of the pointer to free
 *
 * Return: 1 if memory is released, otherwise 0.
 */
int releaseMemory(void **memoryPtr)
{
    if (memoryPtr && *memoryPtr)
    {
        free(*memoryPtr);
        *memoryPtr = NULL;
        return 1;
    }
    return 0;
}
