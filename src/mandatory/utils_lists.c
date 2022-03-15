/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lists.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:26:59 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 17:19:56 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd_middle(t_env **new, t_env **tmp)
{
	t_env	*next;

	next = (*tmp)->next;
	(*tmp)->next = *new;
	(*new)->next = next;
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*copy;

	// if (!lst || !new)
	// 	return ;
	if (*lst != NULL)
	{
		copy = *lst;
		while (copy->next != NULL)
			copy = copy->next;
		copy->next = new;
	}
	else
		*lst = new;
}

void	ft_lstadd_front(t_env **lst, t_env *new)
{
	if (*lst == NULL)
		*lst = new;
	else
	{
		new->next = *lst;
		*lst = new;
	}
}

U_INT	ft_lstsize(t_env *lst)
{
	U_INT	len;

	len = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		len++;
	}
	return (len);
}

void	lstadd_back(t_templ **lst, t_templ *new)
{
	t_templ	*copy;

	if (!lst || !new)
		return ;
	if (*lst != NULL)
	{
		copy = *lst;
		while (copy->next != NULL)
			copy = copy->next;
		copy->next = new;
	}
	else
		*lst = new;
}

int	dellist(t_env **list, t_env **c_list)
{
	t_env	*copy;

	if (*list == *c_list)
	{
		*list = (*list)->next;
		free(*c_list);
		*c_list = NULL;
		return (0);
	}
	copy = *list;
	while (copy->next != *c_list && copy->next != NULL)
	{
		copy = copy->next;
		if (copy->next == *c_list)
		{
			copy->next = (*c_list)->next;
			free(*c_list);
			*c_list = NULL;
		}
	}
	return (0);
}
