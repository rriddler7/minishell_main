/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lists_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:26:59 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:59:15 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
