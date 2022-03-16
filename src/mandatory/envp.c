/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:10:51 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 23:27:11 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_list_back(t_env **new, t_input	*input)
{
	t_env	*copy;

	if (input->envp == NULL)
	{
		input->envp = *new;
		return ;
	}
	else
	{
		copy = input->envp;
		while (copy->next)
			copy = copy->next;
		copy->next = *new;
		(*new)->next = NULL;
	}
}

void	add_list_back_star(t_env **new, t_input	*input)
{
	t_env	*copy;

	if (input->star == NULL)
	{
		input->star = *new;
		return ;
	}
	else
	{
		copy = input->star;
		while (copy->next)
			copy = copy->next;
		copy->next = *new;
		(*new)->next = NULL;
	}
}

t_env	*create_new_list(char *str, t_input *input)
{
	U_INT	i;
	t_env	*tmp;

	tmp = malloc(sizeof(t_env));
	if (tmp == NULL)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	tmp->key = modif_substr(str, 0, i, input);
	if (str[i] && str[i] == '=')
		tmp->equal = 1;
	if (!str[i] || !str[i + 1])
		tmp->value = modif_strdup("\0", input);
	else
		tmp->value = modif_substr(str, i + 1, ft_strlen(str) - i + 1, input);
	tmp->next = NULL;
	return (tmp);
}

static inline char	**make_env_array2(t_input *input, U_INT counter, U_INT i)
{
	char	*array;
	char	**arg;
	t_env	*copy;

	arg = malloc(sizeof(char *) * (counter + 1));
	if (!arg)
		print_error(input, 12, "malloc", NULL);
	copy = input->envp;
	while (i < counter)
	{
		if (copy->value[0])
			array = ft_strjoin_for_3(copy->key, "=", copy->value, input);
		else
			array = modif_strdup(copy->key, input);
		arg[i] = array;
		i++;
		copy = copy->next;
	}
	arg[i] = NULL;
	return (arg);
}

void	make_env_array(t_input *input, char ***full_envp)
{
	U_INT	counter;
	t_env	*copy;

	copy = input->envp;
	counter = 0;
	while (copy)
	{
		copy = copy->next;
		counter++;
	}
	free(copy);
	input->arg_env = make_env_array2(input, counter, 0);
}
