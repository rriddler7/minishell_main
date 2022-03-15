/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:44:38 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 22:19:32 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline t_status	check_quotes(t_input *input, char *str)
{
	U_INT	i;

	i = 0;
	// if (str[i] == '|')
	// 	return (fail);
	// while (str[i])
	// {
	// 	if (str[i] == '|')
	// 		i++;
	// 	while (str[i] == ' ')
	// 		i++;
	// 	if (!str[i] || str[i] == '|')
	// 		return (fail);
		while (str[i] && str[i] != '|')
		{
			if (str[i] == '\'' || str[i] == '\"')
			{
				if (go_to_end_quote(str, &i, str[i], input) == fail)
					return (print_token(input, "newline", 0));
			}
			else
				i++;
		}
	// }
	return (success);
}

static inline void	check_red2(t_input *input, char *str_comman, U_INT *i)
{
	while (str_comman[*i] && str_comman[*i] != '<' && \
	str_comman[*i] != '>')
	{
		if (str_comman[*i] == '\'' || str_comman[*i] == '\"')
			go_to_end_quote(str_comman, i, str_comman[*i], input);
		else
			(*i)++;
	}
}

static inline t_status	check_red(t_input *input, char *str_comman)
{
	U_INT	i;

	i = 0;
	check_red2(input, str_comman, &i);
	while (str_comman[i])
	{
		if (str_comman[i])
			i++;
		if (str_comman[i] == str_comman[i - 1])
			i++;
		while (str_comman[i] == ' ')
			i++;
		if (!str_comman[i] || str_comman[i] == '<' || \
		str_comman[i] == '>' || str_comman[i] == '|')
			return (fail);
		check_red2(input, str_comman, &i);
	}
	return (success);
}

t_status	finder2(char *str_command, t_input *input, U_INT *i)
{
	U_INT	j;

	while (str_command[*i] == ' ')
		++i;
	if (str_command[*i] == '|' || str_command[*i] == '&' || \
	str_command[*i] == ';')
	{
		j = *i;
		++(*i);
		while (str_command[*i] && str_command[*i] == str_command[j])
			++(*i);
		while (str_command[*i] && str_command[*i] == ' ')
			++(*i);
		while (str_command[*i] && str_command[*i] == str_command[j])
		{
			j = *i;
			++(*i);
		}
		if (str_command[*i] == '\0')
		{
			input->num_error = 258;
			return (print_token(input, &str_command[j], 0));
		}
	}
	return (success);
}

t_status	finder(char *str_command, t_input *input, U_INT i)
{
	U_INT	j;

	finder2(str_command, input, &i);
	if (str_command[i] == '<' || str_command[i] == '>')
	{
		j = i;
		++i;
		while ((str_command[i]) && (str_command[i] == str_command[j] || \
		str_command[i] == ' '))
			++i;
		while (str_command[i] && str_command[i] == str_command[j])
			++i;
		while (str_command[i] && str_command[i] == ' ')
			++i;
		while (str_command[i] && str_command[i] == str_command[j])
			++i;
		if (str_command[i] == '\0')
		{
			input->num_error = 258;
			return (print_token(input, "newline", 0));
		}
	}
	check_quotes(input, str_command);
	return (success);
}
