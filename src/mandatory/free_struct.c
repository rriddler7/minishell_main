/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:22:34 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 21:40:23 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_star(t_input *input)
{
	t_env	*copy;

	if (input->have_star == 0 || !input->star)
		return ;
	input->have_star = 0;
	while (input->star != NULL)
	{
		copy = input->star;
		input->star = input->star->next;
		free(copy->key);
		free(copy->value);
		free(copy);
	}
	input->star = NULL;
}

static inline void	free_arg_env(t_input *input)
{
	U_INT	i;

	i = 0;
	if (!input->arg_env)
		return ;
	while (input->arg_env[i])
	{
		free(input->arg_env[i]);
		i++;
	}
	free(input->arg_env);
	input->arg_env = NULL;
}

void	free_all(t_input *input)
{
	if (input->arg_env)
		free_arg_env(input);
	if (input->token)
		free(input->token);
	if (input->direct)
		free_direct(input);
	if (input->envp)
		free_env(input);
	if (input->command)
		free_t_comm(input);
	init_input(input);
}

void	free_direct(t_input *input)
{
	t_direct	*copy;

	if (!input->direct)
		return ;
	while (input->direct)
	{
		copy = input->direct;
		input->direct = input->direct->next;
		if (copy->incoming == 1 && copy->twin == 1)
			unlink(copy->name);
		free(copy->name);
		free(copy->stop_word);
		free(copy);
	}
	input->direct = NULL;
}

void	free_str_command(char *str_command, t_input *input, U_INT i) //free_fd_env
{
	free(str_command);
	free_direct(input);
	free_t_comm(input);
	if (input->fd)
	{
		while (input->fd && input->fd[i])
			free(input->fd[i++]);
		free(input->fd);
		input->fd = NULL;
	}
	input->num_of_command = 0;
	free_arg_env(input);
	if (input->star)
		free_star(input);
}
