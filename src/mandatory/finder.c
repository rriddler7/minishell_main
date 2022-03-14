/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:44:38 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 22:27:27 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline t_status	check_quotes(t_input *input, char *str)
{
	U_INT	i;

	i = 0;
	if (str[i] == '|')
		return (fail);
	while (str[i])
	{
		if (str[i] == '|')
			i++;
		while (str[i] == ' ')
			i++;
		if (!str[i] || str[i] == '|')
			return (fail);
		while (str[i] && str[i] != '|')
		{
			if (str[i] == '\'' || str[i] == '\"')
			{
				if (go_to_end_quote(str, &i, str[i], input) == fail)
					return (print_token(input, "newline"));
			}
			else
				i++;
		}
	}
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

t_status	finder(char *str_command, t_input *input, U_INT i)
{
	if (check_quotes(input, str_command) == fail)
	// if (check_quotes(input, str_command) == fail || \
	// check_red(input, str_command) == fail)
	{
		input->num_error = 258;
		// input->token = modif_strdup("newline", input);
		return (fail);
	}
	// 	return (fail);
	while (str_command[i] == ' ')
		i++;
	if (str_command[i] == '|')
	{
		if (str_command[i + 1] && str_command[i + 1] == '|')
			return (print_token(input, "||"));
		else
			return (print_token(input, "|"));
	}
	while (str_command[i] && str_command[i] != '|')
		i++;
	if (!str_command[i] && (str_command[i] == '|' || \
	str_command[i] == '&' || str_command[i] == '>' || \
	str_command[i] == '<'))
	{
		input->num_error = 258;
		// return (fail);
		return (print_token(input, &str_command[i]));
	}
	return (success);
}
