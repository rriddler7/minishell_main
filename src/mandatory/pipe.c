/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 15:16:49 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/13 21:22:50 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd(t_input *input, int *fd_file, U_INT counter, U_INT i)
{
	while (i < counter)
	{
		if (!input->fd || !input->fd[i])
			continue ;
		close(input->fd[i][0]);
		close(input->fd[i][1]);
		free(input->fd[i]);
		i++;
	}
	free(input->fd);
	input->fd = NULL;
	if (fd_file)
	{
		if (fd_file[0])
			close(fd_file[0]);
		if (fd_file[1])
			close(fd_file[1]);
	}
}

void	clean_path(char **path, U_INT i) //free_path
{
	while (path != NULL && path[i] != NULL)
	{
		free(path[i]);
		i++;
	}
	free(path);
}

void	modif_wait(pid_t *id, U_INT counter, t_input *input, U_INT i)
{
	int	exit_status;

	while (i < counter)
	{
		waitpid(-1, &exit_status, 0);
		i++;
	}
	if (input != NULL)
	{
		if (WIFEXITED(exit_status))
		{
			input->num_error = WEXITSTATUS(exit_status);
			// printf("дочка завершилась хорошо, через exit - input->num_error = %d, errno = %d\n", input->num_error, errno);
		}
	}
	else if (WIFSIGNALED(exit_status))
	{
		input->num_error = WTERMSIG(exit_status);
		// printf("дочка завершилась хорошо, через сигнал - input->num_error = %d, errno = %d\n", input->num_error, errno);
	}
	else
	{
		input->num_error = exit_status + 128;
				// printf("дочка завершилась ПЛОХО!!!! - input->num_error = %d, errno = %d\n", input->num_error, errno);
	}
	free(id);
}

void	make_fork(t_input *input, t_comm *command, U_INT i)
{
	pid_t	*id;

	id = malloc(sizeof(pid_t) * input->num_of_command);
	if (!id)
		print_error(input, errno, "malloc", NULL);
	while (i < input->num_of_command)
	{
		in_signal(input, i, input->command, 0);
		id[i] = fork();
		if (id[i] == 0)
			it_is_child(input, i, 0);
		else if (id[i] == -1)
		{
			input->num_error = errno;
			close_fd(input, NULL, input->num_of_command - 1, 0);
			modif_wait(id, i, input, 0);
			print_error(input, input->num_error, "fork", NULL);
		}
		i++;
	}
	close_fd(input, NULL, input->num_of_command - 1, 0);
	modif_wait(id, input->num_of_command, input, 0);
}

void	open_pipes(t_input *input)
{
	int	i;

	input->fd = malloc(sizeof(int *) * (input->num_of_command - 1));
	if (!input->fd)
		print_error(input, errno, "malloc", NULL);
	i = 0;
	while (i < (input->num_of_command - 1))
	{
		input->fd[i] = malloc(sizeof(int) * 2);
		if (!input->fd[i])
			print_error(input, errno, "malloc", NULL);
		if (pipe(input->fd[i]) == -1)
		{
			input->num_error = errno;
			while (i--)
			{
				close(input->fd[i][0]);
				close(input->fd[i][1]);
			}
			print_error(input, errno, "pipe", NULL);
		}
		i++;
	}
}
