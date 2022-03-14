/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 21:20:59 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 00:20:13 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*ft_lstnew_env(t_env *env, t_input *input)
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

static int	only_export(t_input *input)
{
	t_env	*copy;

	copy = sort_export(input);
	while (copy)
	{
		if (copy->equal == 1)
			printf("declare -x %s%s\"%s\"\n", copy->key, "=", copy->value);
		else
			printf("declare -x %s\n", copy->key);
		copy = copy->next;
	}
	free_new(&copy);
	return (0);
}

static void	change_env(t_env **env, t_env *new, t_input *input)
{
	t_env	*copy2;
	t_env	*lstnew;

	while (new)
	{
		copy2 = *env;
		while (copy2)
		{
			if (!ft_strcmp(new->key, copy2->key))
			{
				free(copy2->value);
				copy2->value = modif_strdup(new->value, input);
				free(new->value);
				free(new->key);
				free(new);
				break ;
			}
			else
				copy2 = copy2->next;
		}
		if (!copy2)
			ft_lstadd_back(&input->envp, new);
		new = new->next;
	}
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
			printf("%s%s%s\n", "export: `", \
				command->words[i], "': not a valid identifier");
		}
		i++;
	}
	change_env(&input->envp, new, input);
	free_new(&new);
	// free_arg_env(input);
	// my_free_t_comm(&command);
	return (input->num_error);
}
