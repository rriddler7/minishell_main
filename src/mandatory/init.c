/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:14:27 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 01:41:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_input(t_input *input)
{
	input->num_of_command = 0;
	input->arg_env = NULL;
	input->fd = NULL;
	input->num_error = 0;
	input->token = NULL;
    input->command = NULL;
	input->direct = NULL;
	input->envp = NULL;
}

void	direct_init(t_direct *new, U_INT *i, U_INT j, char *str)
{
	new->name = NULL;
	new->stop_word = NULL;
	new->value = j;
	new->incoming = 0;
	new->twin = 0;
	new->next = NULL;
}

void mark_direct(t_input *input)
{
	U_INT	i;

	if (!input->command)
		return ;

	i = 0;
	while (input->direct)
	{
		while (i < input->direct->value)
			{
				input->command = input->command->next;
				i++;
			}
		if (input->direct->incoming == 1)
			input->command->direct_in = input->direct;
		else
			input->command->direct_out = input->direct;
		input->direct = input->direct->next;
	}
}

void choose_build(t_comm *command)
{
	t_comm *copy;

	copy = command;
	while (copy != NULL)
	{
		if (copy->words[0] == NULL)
			continue;
		else
		{
			if (ft_strcmp(copy->words[0], "echo") == 0)
				copy->build_number = 1;
			else if (ft_strcmp(copy->words[0], "cd") == 0)
				copy->build_number = 2;
			else if (ft_strcmp(copy->words[0], "pwd") == 0)
				copy->build_number = 3;
			else if (ft_strcmp(copy->words[0], "export") == 0)
				copy->build_number = 4;
			else if (ft_strcmp(copy->words[0], "unset") == 0)
				copy->build_number = 5;
			else if (ft_strcmp(copy->words[0], "env") == 0)
				copy->build_number = 6;
			else if (ft_strcmp(copy->words[0], "exit") == 0)
				copy->build_number = 7;
			else if (ft_strcmp(copy->words[0], "*") == 0)
				copy->build_number = -1;
			
		}
		copy = copy->next;
	}
}