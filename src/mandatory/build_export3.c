/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 22:48:00 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 23:15:23 by pveeta           ###   ########.fr       */
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
