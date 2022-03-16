/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_open_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:17:22 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:54:45 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static inline t_status	try_open3(t_direct	*copy, t_input *input)
{
	int	fd;

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

	copy = input->direct;
	while (copy != NULL)
	{
		if (copy->incoming == 0 || copy->twin == 0)
		{
			if (try_open3(copy, input) == fail)
				return (fail);
		}
		else
			if (launch_heredoc(copy, input, 0) == fail)
				return (fail);
		copy = copy->next;
	}
	return (success);
}

void	try_open(t_input *input)
{
	int		fd;
	t_comm	*copy;

	if (try_open2(input) == fail)
		return ;
	copy = input->command;
	while (copy)
	{
		input->num_of_command++;
		copy = copy->next;
	}
	if (input->num_of_command > 1)
		open_pipes(input);
	if ((input->num_of_command == 1 && (input->command->build_number && \
	!input->command->direct_out)) || (input->command->build_number == 7))
		input->num_error = launcher(input, input->command);
	else if (input->num_of_command != 1 || input->command->words[0] != NULL \
	|| (input->command->build_number && input->command->direct_out))
		make_fork(input, input->command, 0);
}
