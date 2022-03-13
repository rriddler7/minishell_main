/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_other3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 21:09:06 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/13 20:15:00 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(char *str, char c)
{
	U_INT	i;

	i = 0;
	while (*(str + i) != 0)
	{
		if (*(str + i) != c)
			i++;
		else
			return (str + i);
	}
	return (NULL);
}

void	iter_str(char *str, U_INT *i)
{
	while (str[*i] && str[*i] != '\0' && str[*i] != '|' && str[*i] != ' ')
		(*i)++;
}

void	my_gnl(char **new_line, t_input *input)
{
	char	*tmp;
	char	*finish;

	*new_line = modif_strdup("\0", input);
	finish = modif_strdup("\0", input);
	while (finish[0] != '\n')
	{
		read(0, finish, 1);
		if (finish[0] == '\n')
			break ;
		tmp = ft_strjoin(*new_line, finish, input);
		if (tmp == NULL)
		{
			free(finish);
			print_error(input, 12, "malloc", NULL);
		}
		free(*new_line);
		*new_line = tmp;
	}
	free(finish);
}
