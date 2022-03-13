/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_t_comm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:27:46 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/17 20:27:46 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	go_through_redirect(char *str, U_INT *i, char c, t_input *input)
{
	(*i)++;
	if (str[*i] == c)
		(*i)++;
	while (str[*i] == ' ')
		(*i)++;
	while (str[*i] && str[*i] != '>' && str[*i] != '<' \
		&& str[*i] != '|' && str[*i] != ' ')
	{
		if (str[*i] == '\'')
			go_to_end_quote(str, i, '\'', input);
		if (str[*i] == '\"')
			go_to_end_quote(str, i, '\"', input);
		else
			(*i)++;
	}
}

U_INT	get_number_of_words(char *str, U_INT *i, t_input *input, U_INT counter)
{
	while (str[*i] != '\0' && str[*i] != '|')
	{
		if (str[*i] == ' ')
			(*i)++;
		else if (str[*i] == '<')
			go_through_redirect(str, i, '<', input);
		else if (str[*i] == '>')
			go_through_redirect(str, i, '>', input);
		else if (str[*i] == '*')
		{
			if (counter == 0)
				counter++;
			else
				counter += ft_lstsize(input->star);
			while (str[*i] && str[*i] != '\0' && str[*i] != '|' \
			&& str[*i] != ' ')
				(*i)++;
		}
		else
		{
			counter++;
			go_through_word(str, i, input);
		}
	}
	return (counter);
}

t_status star_in_str(char *str,  t_input *input)
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

static t_comm	*create_list_comm(char *str, t_input *input, U_INT *i)
{
	U_INT	counter;
	t_comm	*new;

	new = malloc(sizeof(t_comm));
	if (new == NULL)
		return (NULL);
	new->direct_in = NULL;
	new->direct_out = NULL;
	new->next = NULL;
	new->build_number = 0;
	if (star_in_str(str, input) == success)
		find_star_in_comm(input);
	counter = get_number_of_words(str, i, input, 0);
	new->words = malloc(sizeof(char *) * (counter + 1));
	if (!new->words)
		return (NULL);
	new->words[0] = NULL;
	return (new);
}

static void	add_comm_back(t_comm *new, t_comm **command)
{
	t_comm	*copy;

	if (*command == NULL)
		*command = new;
	else
	{
		copy = *command;
		while (copy->next != NULL)
			copy = copy->next;
		copy->next = new;
	}
}

t_status	create_empty_t_comm(char *str_command, t_input *input)
{
	U_INT	i;
	t_comm	*new;

	i = 0;
	while (str_command[i])
	{
		while (str_command[i] && str_command[i] != '|')
		{
			new = create_list_comm(str_command, input, &i);
			if (!new)
				return (fail);
			add_comm_back(new, &input->command);
		}
		if (str_command[i])
			i++;
	}
	if (!str_command[i] && (str_command[i - 1] == '|' || \
		str_command[i - 1] == '&' || str_command[i - 1] == '>' || \
		str_command[i - 1] == '<'))
		return (end);
	return (success);
}
