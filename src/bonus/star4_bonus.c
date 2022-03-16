/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star4_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 19:25:15 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:58:22 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_status	add_list_support(char *str, U_INT *i, t_templ **copy, t_status f)
{
	if (f == 0)
	{
		if (str[*i + 1])
			*copy = (*copy)->next;
		else
		{
			(*copy)->next = NULL;
			return (fail);
		}
	}
	else
	{
		while (str[*i] && str[*i] == '*')
			++(*i);
		--(*i);
		if (str[*i + 1] && str[*i + 1] != '*')
			*copy = (*copy)->next;
		else
		{
			(*copy)->next = NULL;
			++(*i);
		}
	}
	return (success);
}

t_status	add_list_in_templ2(t_templ **temp, char *str, \
t_input *input, U_INT *i)
{
	U_INT	m;

	m = *i;
	if (str[0] != '*')
	{
		(*temp)->status = 1;
		while (str[*i] && str[*i] != '*')
			++(*i);
		(*temp)->value = modif_substr(str, m, *i - m, input);
		if (add_list_support(str, i, temp, 0) == fail)
			return (fail);
	}
	return (success);
}

void	add_list_in_templ(t_templ *temp, char *str, t_input *input, U_INT i)
{
	U_INT	m;
	U_INT	n;

	while (str[i])
	{
		if (add_list_in_templ2(&temp, str, input, &i) == fail)
			return ;
		if (str[i] == '*')
		{
			while (str[i] == '*')
				++i;
			n = i;
			m = i;
			temp->status = 3;
			while (str[n])
			{
				if (str[n++] == '*')
					temp->status = 2;
			}
			while (str[i] && str[i] != '*')
				++i;
			temp->value = modif_substr(str, m, i - m, input);
			add_list_support(str, &i, &temp, 1);
		}
	}
}
