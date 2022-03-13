/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 19:22:54 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/13 19:23:16 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_status_one(t_templ **copy_templ, U_INT *i, char *str)
{
	if ((*copy_templ)->value[0] != str[0] || \
	ft_strncmp((*copy_templ)->value, str, ft_strlen((*copy_templ)->value)) != 0)
		return (fail);
	*i += ft_strlen((*copy_templ)->value);
	if ((*copy_templ)->next)
		*copy_templ = (*copy_templ)->next;
	else
		return (success);
	return (3);
}

int	check_status_two(t_templ **copy_templ, U_INT *i, char *str)
{
	t_templ	*last;

	while (str[*i] && *copy_templ && (*copy_templ)->value && \
	(*copy_templ)->status == 2)
	{
		if (ft_strncmp((*copy_templ)->value, str + *i, \
		ft_strlen((*copy_templ)->value)) == 0)
		{
			*i += ft_strlen((*copy_templ)->value);
			if (!((*copy_templ)->next))
				return (success);
			else if ((*copy_templ)->next)
			{
				(*copy_templ) = (*copy_templ)->next;
				if ((*copy_templ)->status == 3)
					return (4);
			}
		}
		else
			++(*i);
	}
	if ((*copy_templ) && (*copy_templ)->status != 3)
		return (fail);
	return (3);
}

int	check_status_three(t_templ **copy_templ, U_INT *i, char *str)
{
	while (str[*i] && *copy_templ && (*copy_templ)->value)
	{
		if (ft_strncmp((*copy_templ)->value, str + *i, \
		ft_strlen((*copy_templ)->value)) == 0)
		{
			*i += ft_strlen((*copy_templ)->value);
			if (!((*copy_templ))->next && !str[*i])
				return (success);
			else if ((*copy_templ)->next)
				(*copy_templ) = (*copy_templ)->next;
		}
		else
			++(*i);
	}
	if ((*copy_templ) || str[*i])
		return (fail);
	return (success);
}

t_status	check_template(t_input *input, char *str, t_templ *temple, U_INT i)
{	
	int		itog;

	itog = 0;
	while (str[i])
	{
		if (temple->status == 1)
		{
			itog = check_status_one(&temple, &i, str);
			if (itog == fail || itog == success)
				return (itog);
		}
		while (temple->status == 2)
		{
			itog = check_status_two(&temple, &i, str);
			if (itog == fail || itog == success)
				return (itog);
		}
		if (temple->status == 3)
		{
			itog = check_status_three(&temple, &i, str);
			if (itog == fail || itog == success)
				return (itog);
		}
	}
	return (success);
}
