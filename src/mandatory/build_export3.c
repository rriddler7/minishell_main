/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 22:48:00 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 23:20:24 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

t_status	check_word(char *str)
{
	U_INT	i;

	i = 0;
	if ((str[0] != '_') && !ft_isalpha(str[0]))
		return (fail);
	while (str[i] && str[i] != '=')
	{
		if ((str[i] != '_') && !ft_isalpha(str[i]) \
		&& !ft_isdigit(str[i]))
			return (fail);
		++i;
	}
	return (success);
}

int	only_export(t_input *input)
{
	t_env	*copy;
	t_env	*start;

	copy = sort_export(input);
	start = copy;
	while (copy)
	{
		if (copy->equal == 1)
			printf("declare -x %s%s\"%s\"\n", copy->key, "=", copy->value);
		else
			printf("declare -x %s\n", copy->key);
		copy = copy->next;
	}
	free_new(&start);
	return (0);
}

void	ft_lstadd_back2(t_env **lst, t_env *new, t_input *input)
{
	t_env	*copy;
	t_env	*tmp;

	if (!lst || !new)
		return ;
	tmp = ft_lstnew_env(new, input);
	if (*lst != NULL)
	{
		copy = *lst;
		while (copy->next != NULL)
			copy = copy->next;
		copy->next = tmp;
	}
	else
		*lst = tmp;
}
