/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 18:32:46 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 21:30:12 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	put_shlvl2(t_input *input, U_INT level)
{
	t_env	*copy;
	int		flag;

	copy = input->envp;
	flag = 0;
	while (copy != NULL)
	{
		if (ft_strcmp(copy->key, "SHLVL") == 0 && copy->value[0] == '\0')
		{
			free(copy->value);
			copy->value = modif_itoa(level, input);
			return ;
		}
		else if (ft_strcmp(copy->key, "SHLVL") == 0 && copy->value[0] != '\0')
		{
			level += modif_atoi(copy->value, &flag, 0);
			free(copy->value);
			copy->value = modif_itoa(level, input);
			return ;
		}
		copy = copy->next;
	}
}

void	put_shlvl(t_input *input)
{
	t_env	*copy;

	copy = input->envp;
	if (!copy)
	{
		copy = malloc(sizeof(t_env));
		if (copy == NULL)
			print_error(input, 12, "malloc", NULL);
		copy->key = modif_strdup("SHLVL", input);
		copy->value = modif_strdup("1", input);
		input->envp = copy;
		copy->next = NULL;
	}
	else
		put_shlvl2(input, 1);
}
