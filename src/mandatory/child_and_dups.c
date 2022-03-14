/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_and_dups.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 15:19:35 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 23:40:19 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// void	my_free_t_comm(t_comm **command)
// {
// 	U_INT	i;
// 	t_comm	*copy;

// 	while (command)
// 	{
// 		i = 0;
// 		copy = *command;
// 		*command = (*command)->next;
// 		while (copy->words[i])
// 			free(copy->words[i++]);
// 		free(copy->words);
// 		free(copy);
// 	}
// 	free(*command);
// 	*command = NULL;
// }

void	it_is_child(t_input *input, U_INT i, U_INT counter)
{
	t_comm	*copy;
	char	*path;
	char	*str;

	if (it_is_child2(input, &copy, counter, &i) == fail)
		exit(0);
	str = ft_strjoin(copy->words[0], ": command not found", input);
	child_dup(input, copy, i);
	if (copy->build_number != 0)
	{
		launcher(input, copy); //++++
		// exit(launcher(input, copy);
		// my_free_t_comm(&copy);
	}
	else
	{
		if (copy->next && copy->next->words[0] && \
		ft_strcmp(copy->next->words[0], "exit") == 0)
			exit(7);
		path = get_path(copy->words[0], input, 0);
		if (!path)
			print_error(input, 127, NULL, str);
		if (execve(path, copy->words, input->arg_env) != 0)
			print_error(input, 127, NULL, str);
	}
	free(str);
	exit (input->num_error);
}

