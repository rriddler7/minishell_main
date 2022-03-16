/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 21:20:59 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:49:59 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static t_env	*create_new_list2(char *str, t_input *input)
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
	free(str);
	return (tmp);
}

t_env	*create_sort_env(t_env *old, t_input *input)
{
	t_env	*new;

	if (old->equal == 1)
		new = create_new_list2(ft_strjoin_for_3 (old->key, \
			"=", old->value, input), input);
	else
		new = create_new_list2(modif_strdup(old->key, input), input);
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
