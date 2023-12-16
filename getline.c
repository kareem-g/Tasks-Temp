#include "shell.h"

/**
 * bufferChainedCommands - buffers chained commands
 * @info: parameter struct
 * @commandBuffer: address of buffer
 * @bufferLength: address of length variable
 *
 * Return: bytes read
 */
ssize_t bufferChainedCommands(info_t *info, char **commandBuffer, size_t *bufferLength)
{
	ssize_t bytesRead = 0;
	size_t bufferPosition = 0;

	if (!*bufferLength) /* if nothing left in the buffer, fill it */
	{
		free(*commandBuffer);
		*commandBuffer = NULL;
		signal(SIGINT, handleInterrupt);
#if USE_GETLINE
		bytesRead = getline(commandBuffer, bufferLength, stdin);
#else
		bytesRead = customGetLine(info, commandBuffer, bufferLength);
#endif
		if (bytesRead > 0)
		{
			if ((*commandBuffer)[bytesRead - 1] == '\n')
			{
				(*commandBuffer)[bytesRead - 1] = '\0'; /* remove trailing newline */
				bytesRead--;
			}
			info->lineCountFlag = 1;
			removeComments(*commandBuffer);
			buildHistoryList(info, *commandBuffer, info->historyCount++);
			/* if (_strchr(*commandBuffer, ';')) is this a command chain? */
			{
				*bufferLength = bytesRead;
				info->commandBuffer = commandBuffer;
			}
		}
	}
	return bytesRead;
}

/**
 * getInput - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t getInput(info_t *info)
{
	static char *commandBuffer; /* the ';' command chain buffer */
	static size_t position_i, position_j, bufferLength;
	ssize_t bytesRead = 0;
	char **bufferPointer = &(info->arguments), *currentPosition;

	_putchar(BUF_FLUSH);
	bytesRead = bufferChainedCommands(info, &commandBuffer, &bufferLength);
	if (bytesRead == -1) /* EOF */
		return -1;
	if (bufferLength)	/* we have commands left in the chain buffer */
	{
		position_j = position_i; /* init new iterator to current buffer position */
		currentPosition = commandBuffer + position_i; /* get pointer for return */

		checkCommandChain(info, commandBuffer, &position_j, position_i, bufferLength);
		while (position_j < bufferLength) /* iterate to semicolon or end */
		{
			if (isCommandChain(info, commandBuffer, &position_j))
				break;
			position_j++;
		}

		position_i = position_j + 1; /* increment past nulled ';'' */
		if (position_i >= bufferLength) /* reached end of buffer? */
		{
			position_i = bufferLength = 0; /* reset position and length */
			info->commandBufferType = CMD_NORM;
		}

		*bufferPointer = currentPosition; /* pass back pointer to current command position */
		return _strlen(currentPosition); /* return length of current command */
	}

	*bufferPointer = commandBuffer; /* else not a chain, pass back buffer from customGetLine() */
	return bytesRead; /* return length of buffer from customGetLine() */
}

/**
 * readBuffer - reads a buffer
 * @info: parameter struct
 * @buffer: buffer
 * @position_i: size
 *
 * Return: bytesRead
 */
ssize_t readBuffer(info_t *info, char *buffer, size_t *position_i)
{
	ssize_t bytesRead = 0;

	if (*position_i)
		return 0;
	bytesRead = read(info->readfd, buffer, READ_BUF_SIZE);
	if (bytesRead >= 0)
		*position_i = bytesRead;
	return bytesRead;
}

/**
 * customGetLine - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: size
 */
int customGetLine(info_t *info, char **ptr, size_t *length)
{
	static char buffer[READ_BUF_SIZE];
	static size_t position_i, bufferLength;
	size_t k;
	ssize_t bytesRead = 0, size = 0;
	char *currentPosition = NULL, *newPosition = NULL, *c;

	currentPosition = *ptr;
	if (currentPosition && length)
		size = *length;
	if (position_i == bufferLength)
		position_i = bufferLength = 0;

	bytesRead = readBuffer(info, buffer, &bufferLength);
	if (bytesRead == -1 || (bytesRead == 0 && bufferLength == 0))
		return -1;

	c = _strchr(buffer + position_i, '\n');
	k = c ? 1 + (unsigned int)(c - buffer) : bufferLength;
	newPosition = _realloc(currentPosition, size, size ? size + k : k + 1);
	if (!newPosition) /* MALLOC FAILURE! */
		return (currentPosition ? free(currentPosition), -1 : -1);

	if (size)
		_strncat(newPosition, buffer + position_i, k - position_i);
	else
		_strncpy(newPosition, buffer + position_i, k - position_i + 1);

	size += k - position_i;
	position_i = k;
	currentPosition = newPosition;

	if (length)
		*length = size;
	*ptr = currentPosition;
	return size;
}

/**
 * handleInterrupt - blocks ctrl-C
 * @sigNum: the signal number
 *
 * Return: void
 */
void handleInterrupt(__attribute__((unused))int sigNum)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
