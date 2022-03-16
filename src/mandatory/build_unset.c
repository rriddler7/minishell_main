/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:53 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 23:20:29 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_key(t_env **env, char *str)
{
	t_env	*copy;

	copy = *env;
	if (!ft_strcmp(str, copy->key))
	{
		*env = (*env)->next;
		free(copy->key);
		free(copy->value);
		free(copy);
		return (1);
	}
	return (0);
}

void	remove_env(t_env *env)
{
	t_env	*copy;

	copy = env->next;
	env->next = copy->next;
	free(copy->key);
	free(copy->value);
	free(copy);
}

t_status	launch_unset2(t_input *input, char *str)
{
	if (!input->envp)
		return (fail);
	if (!ft_isalpha(str[0]))
	{
		input->num_error = 1;
		printf("%s%s%s\n", "unset: `", str, \
		"': not a valid identifier");
	}
	return (success);
}

U_INT	launch_unset(t_input *input, t_comm *command)
{
	int		i;
	t_env	*copy1;
	t_env	*copy2;

	i = 1;
	while (command->words[i])
	{
		if (launch_unset2(input, command->words[i]) == fail)
			break ;
		if (command->words[i][0] == '_')
			continue ;
		if (find_key(&input->envp, command->words[i]))
			continue ;
		copy1 = input->envp;
		copy2 = copy1->next;
		while (copy2 && ft_strcmp(command->words[i], copy2->key))
		{
			copy1 = copy2;
			copy2 = copy2->next;
		}
		if (copy2)
			remove_env(copy1);
		i++;
	}
	return (0);
}
