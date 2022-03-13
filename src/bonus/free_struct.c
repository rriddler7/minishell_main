/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:22:34 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/12 17:10:15 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_env(t_input *input)
{
	t_env *copy;

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


void free_all(t_input *input) // аналог void	free_arg(t_arg *arg), но надо доделывать!
{
	free_env(input);
}