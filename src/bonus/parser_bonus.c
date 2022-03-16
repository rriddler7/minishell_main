/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:21:05 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:54:12 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	go_through_word(char *str, U_INT *i, t_input *input)
{
	while (str[*i] && str[*i] != '>' && str[*i] != '<' \
		&& str[*i] != '|' && str[*i] != ' ')
	{
		if (str[*i] == '\'')
			go_to_end_quote(str, i, '\'', input);
		else if (str[*i] == '\"')
			go_to_end_quote(str, i, '\"', input);
		else
			(*i)++;
	}
}

static void	add_command(t_input *input, char *str, U_INT j, U_INT *i)
{
	t_comm	*tmp;
	U_INT	k;
	U_INT	m;

	m = 0;
	k = 0;
	tmp = input->command;
	while (m++ < j)
		tmp = tmp->next;
	while (str[*i] && str[*i] != '|')
	{
		if (str[*i] == ' ')
			(*i)++;
		else if (str[*i] == '>' || str[*i] == '<')
			find_direct_in_comm(input, str, j, i);
		else
		{
			m = *i;
			go_through_word(str, i, input);
			tmp->words[k] = modif_substr(str, m, *i - m, input);
			tmp->words[++k] = NULL;
		}
	}
	if (input->have_star == 1)
		find_star(tmp, input);
}

t_status	parser(char *str_command, t_input *input, U_INT i, U_INT j)
{
	t_status	flag;

	i = 0;
	j = 0;
	flag = create_empty_t_comm(str_command, input);
	if (flag == fail)
	{
		print_error(input, 12, "malloc", NULL);
		return (fail);
	}
	else if (flag == end)
		return (end);
	while (str_command[i])
	{
		if (str_command[i] != '|')
			add_command(input, str_command, j, &i);
		else if (i++ > 0)
			j++;
	}
	return (success);
}
