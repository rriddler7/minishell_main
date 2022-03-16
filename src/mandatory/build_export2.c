/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 21:20:59 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:02:49 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_lstnew_env(t_env *env, t_input *input)
{
	t_env	*newnode;

	newnode = (t_env *)malloc(sizeof(t_env));
	if (newnode == NULL)
		print_error(input, 12, "malloc", NULL);
	newnode->key = modif_strdup(env->key, input);
	newnode->equal = env->equal;
	newnode->value = modif_strdup(env->value, input);
	newnode->next = NULL;
	return (newnode);
}

void	free_new(t_env **new)
{
	t_env	*copy;

	while (*new)
	{
		copy = *new;
		*new = (*new)->next;
		free(copy->key);
		free(copy->value);
		free(copy);
	}
}

static void	change_env2(t_input	*input, t_env *copy_new)
{
	t_env	*copy_envp;

	copy_envp = input->envp;
	while (copy_envp)
	{
		if (ft_strcmp(copy_envp->key, copy_new->key) == success)
		{
			free(copy_envp->key);
			copy_envp->key = modif_strdup(copy_new->value, input);
			return ;
		}
		copy_envp = copy_envp->next;
	}
	if (!copy_envp)
		ft_lstadd_back2(&input->envp, copy_new, input);
}

static void	change_env(t_input	*input, t_env **new)
{
	t_env	*copy_new;

	copy_new = *new;
	while (copy_new)
	{
		change_env2(input, copy_new);
		copy_new = copy_new->next;
	}
	free_new(new);
}

U_INT	launch_export(t_input *input, t_comm *command, U_INT i)
{
	t_env	*new;
	t_env	*tmp;

	input->num_error = 0;
	new = NULL;
	if (command->words[1] == NULL)
		return (only_export(input));
	while (command->words[i])
	{
		if (check_word(command->words[i]) == success)
		{
			tmp = create_new_list(command->words[i], input);
			ft_lstadd_back(&new, tmp);
		}
		else
		{
			input->num_error = 1;
			printf("%s%s%s%s\n", "minishell: ", "export: `", \
				command->words[i], "': not a valid identifier");
		}
		i++;
	}
	change_env(input, &new);
	return (input->num_error);
}
