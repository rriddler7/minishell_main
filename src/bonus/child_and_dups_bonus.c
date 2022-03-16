/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_and_dups_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 15:19:35 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:51:16 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	child_dup2(t_input *input, t_comm *copy, U_INT i, int *fd)
{
	int	res;

	res = 0;
	if (copy->direct_in != NULL)
		res = dup2(fd[0], STDIN_FILENO);
	else if (i != 0 && input->fd)
		res = dup2(input->fd[i - 1][0], STDIN_FILENO);
	if (res == -1)
		print_error(input, errno, "dup2", NULL);
	if (copy->direct_out != NULL)
		res = dup2(fd[1], STDOUT_FILENO);
	else if (input->fd && i < input->num_of_command - 1)
		res = dup2(input->fd[i][1], STDOUT_FILENO);
	if (res == -1)
		print_error(input, errno, "dup2", NULL);
	close_fd(input, fd, input->num_of_command - 1, 0);
}

void	child_dup(t_input *input, t_comm *copy, U_INT i)
{
	int	fd[2];

	fd[0] = -2;
	if (copy->direct_in != NULL)
		fd[0] = open(copy->direct_in->name, O_RDONLY);
	if (fd[0] == -1)
		print_error(input, errno, copy->direct_in->name, NULL);
	fd[1] = -2;
	if (copy->direct_out != NULL && copy->direct_out->twin == 1)
		fd[1] = open(copy->direct_out->name, O_WRONLY | O_CREAT | \
		O_APPEND, 0644);
	else if (copy->direct_out != NULL && copy->direct_out->twin == 0)
		fd[1] = open(copy->direct_out->name, O_WRONLY | O_CREAT | \
		O_TRUNC, 0644);
	if (fd[1] == -1)
		print_error(input, errno, copy->direct_out->name, NULL);
	child_dup2(input, copy, i, fd);
}

t_status	it_is_child2(t_input *input, t_comm	**copy, U_INT counter, U_INT *i)
{
	*copy = input->command;
	signal(SIGINT, handler_child);
	signal(SIGQUIT, handler_child);
	while (counter++ < *i)
		*copy = (*copy)->next;
	if (!(*copy) || !(*copy)->words || !(*copy)->words[0])
		return (fail);
	return (success);
}

void	it_is_child3(t_comm	*copy, t_input *input, char *str)
{
	char	*path;

	if (copy->next && copy->next->words[0] && \
	ft_strcmp(copy->next->words[0], "exit") == 0)
		exit(7);
	path = get_path(copy->words[0], input, 0);
	if (!path)
		print_error(input, 127, NULL, str);
	if (execve(path, copy->words, input->arg_env) != 0)
		print_error(input, 127, NULL, str);
}

void	it_is_child(t_input *input, U_INT i, U_INT counter)
{
	t_comm	*copy;
	char	*str;

	if (it_is_child2(input, &copy, counter, &i) == fail)
		exit(0);
	if (copy->words[0][0] == '/' && access(copy->words[0], F_OK))
	{
		str = ft_strjoin_for_3 (copy->words[0], ": ", strerror(errno), input);
		print_error(input, 126, NULL, ft_strjoin_for_3 (copy->words[0], ": ", \
		strerror(errno), input));
	}
	else
		str = ft_strjoin(copy->words[0], ": command not found", input);
	child_dup(input, copy, i);
	if (copy->build_number != 0)
		launcher(input, copy);
	else
		it_is_child3(copy, input, str);
	free(str);
	exit (input->num_error);
}
