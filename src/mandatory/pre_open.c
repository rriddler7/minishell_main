/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_open.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:17:22 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/13 20:17:15 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline t_status	try_open3(t_direct	*copy, t_input *input)
{
	int	fd;

	// printf("try_open3\n");
	if (copy->incoming == 0 && copy->twin == 1)
		fd = open(copy->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (copy->incoming == 0 && copy->twin == 0)
		fd = open(copy->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(copy->name, O_RDONLY, 0644);
	if (fd == -1)
	{
		input->num_error = errno;
		print_error(input, input->num_error, copy->name, \
		strerror(input->num_error));
		return (fail);
	}
	close(fd);
	return (success);
}

static inline t_status	try_open2(t_input *input)
{
	t_direct	*copy;
	int			fd;

	// printf("try_open2: incoming=%d, twin=%d\n", input->direct->incoming, input->direct->twin);
	copy = input->direct;
	// printf("try_open2\n");
	// printf("try_open2: incoming=%d, twin=%d\n", copy->incoming, copy->twin);
	// if (copy == NULL)
	// 	printf("copy == NULL\n");
	while (copy != NULL)
	{
		// printf("2try_open2\n");
		// printf("2try_open2: incoming=%d, twin=%d\n", copy->incoming, copy->twin);
		if (copy->incoming == 0 || copy->twin == 0)
		{
			if (try_open3(copy, input) == fail)
				return (fail);
		}
		else
			if (launch_heredoc(copy, input, 0) == fail)
			{
				printf("launch_heredoc=fail\n");
				return (fail);
			}
		copy = copy->next;
	}
	return (success);
}

void	try_open(t_input *input)
{
	int		fd;
	t_comm	*copy;

	printf("try_open\n");
	printf("try_open: incoming=%d, twin=%d\n", input->direct->incoming, input->direct->twin);
	if (try_open2(input) == fail)
		return ;
	copy = input->command;
	while (copy)
	{
		input->num_of_command++;
		copy = copy->next;
	}
	printf("try_open: num_of_command=%d, words=%s\n", input->num_of_command, input->command->words[0]);
	if (input->num_of_command > 1)
		open_pipes(input);
	if (input->num_of_command == 1 && (input->command->build_number && !input->command->direct_out))
	{
		// fd = reverse_redir(input);
		input->num_error = launcher(input, input->command);
		// reverse_redir2(input, fd);
	}
	else if (input->num_of_command != 1 || input->command->words[0] != NULL || (input->command->build_number && input->command->direct_out))
		make_fork(input, input->command, 0);
}
