/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 20:46:56 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 21:52:14 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_input *input)
{
	t_env	*copy;

	if (!input->envp)
		return ;
	while (input->envp != NULL)
	{
		copy = input->envp;
		input->envp = input->envp->next;
		free(copy->key);
		free(copy->value);
		free(copy);
	}
	input->envp = NULL;
}

void	free_t_comm(t_input *input)
{
	t_comm	*copy;
	char	*str;
	U_INT	i;

	if (!input || !input->command)
		return ;
	while (input->command)
	{
		i = 0;
		copy = input->command;
		input->command = input->command->next;
		while (copy->words[i])
			free(copy->words[i++]);
		free(copy->words);
		free(copy);
	}
	free(input->command);
	input->command = NULL;
}
