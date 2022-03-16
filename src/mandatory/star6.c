/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 19:30:34 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 23:58:28 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status	n_star(char *str)
{
	U_INT		i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '*')
			return (success);
		++i;
	}
	return (fail);
}

void	free_temple(t_templ *temple)
{
	t_templ	*copy;

	while (temple)
	{
		copy = temple;
		temple = temple->next;
		free(copy->value);
		free(copy);
	}
}

t_status	star_in_str(char *str, t_input *input)
{
	U_INT	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
		{
			input->have_star = 1;
			return (success);
		}
		++i;
	}
	return (fail);
}
