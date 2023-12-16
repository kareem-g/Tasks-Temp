#include "shell.h"

/**
 * custom_shell_loop - main shell loop
 * @info: the parameter & return information structure
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int custom_shell_loop(info_t *info, char **av)
{
	ssize_t input_result = 0;
	int builtin_result = 0;

	while (input_result != -1 && builtin_result != -2)
	{
		clear_information(info);
		if (is_interactive(info))
			print_prompt("$ ");
		flush_buffer();
		input_result = get_user_input(info);
		if (input_result != -1)
		{
			set_information(info, av);
			builtin_result = find_builtin_command(info);
			if (builtin_result == -1)
				find_and_execute_command(info);
		}
		else if (is_interactive(info))
			print_newline();
		free_information(info, 0);
	}
	write_shell_history(info);
	free_information(info, 1);
	if (!is_interactive(info) && info->status)
		exit(info->status);
	if (builtin_result == -2)
	{
		if (info->error_number == -1)
			exit(info->status);
		exit(info->error_number);
	}
	return (builtin_result);
}

/**
 * find_builtin_command - finds a builtin command
 * @info: the parameter & return information structure
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         -2 if builtin signals exit()
 */
int find_builtin_command(info_t *info)
{
	int i, built_in_result = -1;
	builtin_table custom_builtins[] = {
		{"exit", custom_exit},
		{"env", custom_env},
		{"help", custom_help},
		{"history", custom_history},
		{"setenv", custom_setenv},
		{"unsetenv", custom_unsetenv},
		{"cd", custom_cd},
		{"alias", custom_alias},
		{NULL, NULL}
	};

	for (i = 0; custom_builtins[i].type; i++)
		if (compare_strings(info->arguments[0], custom_builtins[i].type) == 0)
		{
			info->line_count++;
			built_in_result = custom_builtins[i].function(info);
			break;
		}
	return (built_in_result);
}

/**
 * find_and_execute_command - finds a command in PATH and executes it
 * @info: the parameter & return information structure
 *
 * Return: void
 */
void find_and_execute_command(info_t *info)
{
	char *path = NULL;

	info->path = info->arguments[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (int i = 0, arg_count = 0; info->arguments[i]; i++)
		if (!is_delimiter(info->arguments[i], " \t\n"))
			arg_count++;
	if (!arg_count)
		return;

	path = find_command_path(info, get_environment_variable(info, "PATH="), info->arguments[0]);
	if (path)
	{
		info->path = path;
		fork_and_execute_command(info);
	}
	else
	{
		if ((is_interactive(info) || get_environment_variable(info, "PATH=")
			|| info->arguments[0][0] == '/') && is_valid_command(info, info->arguments[0]))
			fork_and_execute_command(info);
		else if (*(info->input) != '\n')
		{
			info->status = 127;
			print_error_message(info, "not found\n");
		}
	}
}

/**
 * fork_and_execute_command - forks an exec thread to run a command
 * @info: the parameter & return information structure
 *
 * Return: void
 */
void fork_and_execute_command(info_t *info)
{
	pid_t child_process_id;

	child_process_id = fork();
	if (child_process_id == -1)
	{
		print_error_message(info, "Error:");
		perror("Error:");
		return;
	}
	if (child_process_id == 0)
	{
		if (execute_command(info) == -1)
		{
			free_information(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait_for_child_process(&(info->status));
		if (child_process_exited_normally(info->status))
		{
			info->status = get_exit_status(info->status);
			if (info->status == 126)
				print_error_message(info, "Permission denied\n");
		}
	}
}
