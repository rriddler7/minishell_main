/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 21:20:59 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 00:19:58 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_sort_env(t_env *old, t_input *input)
{
	t_env	*new;

	if (old->equal == 1)
		new = create_new_list(ft_strjoin_for_3 (old->key, \
			"=", old->value, input), input);
	else
		new = create_new_list(modif_strdup(old->key, input), input);
	return (new);
}

t_env	*env_sort(t_env *list, t_env *new, t_input *input)
{
	int		i;
	int		j;
	t_env	*tmp;
	size_t	size;

	i = -1;
	tmp = list;
	while (tmp)
	{
		if (tmp == list && modif_strncmp(tmp->key, new->key, 1) < 0)
			ft_lstadd_front(&list, new);
		if (tmp->next != NULL && modif_strncmp(tmp->key, new->key, 1) > 0 && \
			modif_strncmp(tmp->next->key, new->key, 1) < 0)
			ft_lstadd_middle(&new, &tmp);
		if (tmp->next == NULL && modif_strncmp(tmp->key, new->key, 1) > 0)
			ft_lstadd_back(&list, new);
		tmp = tmp->next;
	}
	free_new(&tmp);
	return (list);
}

t_env	*sort_export(t_input *input)
{
	U_INT	i;
	t_env	*old;
	t_env	*new;
	t_env	*list;

	list = NULL;
	old = input->envp;
	while (old != NULL)
	{
		new = create_sort_env(old, input);
		if (list == NULL)
		{
			list = new;
			old = old->next;
			continue ;
		}
		list = env_sort(list, new, input);
		old = old->next;
	}
	return (list);
}
