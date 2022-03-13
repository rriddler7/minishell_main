/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 20:46:56 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/13 20:47:36 by pveeta           ###   ########.fr       */
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

	i = 0;
	if (!input || !input->command)
		return ;
	while (input->command)
	{
		copy = input->command;
		input->command = input->command->next;
		free(copy);
	}
	input->command = NULL;
}
