/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 17:45:43 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 21:12:45 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_direct_back(t_direct *new, t_direct **direct)
{
	t_direct	*copy;

	if (*direct == NULL)
		*direct = new;
	else
	{
		copy = *direct;
		while (copy->next != NULL)
			copy = copy->next;
		copy->next = new;
	}
}

static void	check_direct(t_input *input, t_direct *new, char *str, U_INT *i)
{
	if (str[*i] == '<')
	{
		new->incoming = 1;
		if (str[*i + 1] == '<')
		{
			new->twin = 1;
			(*i)++;
			if (str[*i] && str[*i] == '>')
				print_token(input, ">", 0);
		}
		else if (str[*i + 1] == '>')
			print_token(input, "newline", 0);
	}
	else if (str[*i] == '>')
	{
		if (str[*i + 1] == '>')
		{
			new->twin = 1;
			(*i)++;
			if (str[*i] && str[*i] == '<')
				print_token(input, "<", 0);
		}
	}
	(*i)++;
}

void	find_direct_in_comm(t_input *input, char *str, U_INT j, U_INT *i)
{
	U_INT		n;
	t_direct	*new;

	new = malloc(sizeof(t_direct));
	if (!new)
		print_error(input, 12, "malloc", NULL);
	direct_init(new, i, j, str);
	check_direct(input, new, str, i);
	// printf("check_direct: twin=%d\n", new->twin);
	while (str[*i] == ' ')
		(*i)++;
	while (str[*i] && str[*i] != '<' && str[*i] != '>' \
		&& str[*i] != '|' && str[*i] != ' ')
	{
		n = *i;
		go_through_word(str, i, input);
		new->name = modif_substr(str, n, *i - n, input);
	}
	add_direct_back(new, &input->direct);
}

void	add_heredoc(t_input *input)
{
	t_direct	*copy;
	char		*command;

	copy = input->direct;
	while (copy)
	{
		printf("add_heredoc: incoming=%d, twin=%d, name=%s\n", copy->incoming, copy->twin, copy->name);
		if (copy->incoming == 1 && copy->twin == 1)
		{
			printf("2add_heredoc: incoming=%d, copy->twin=%d, name=%s\n", copy->incoming, copy->twin, copy->name);
			copy->stop_word = copy->name;
			printf("stop_word=%s\n", copy->stop_word);
			printf("value=%d\n", copy->value);
			if (copy->value != 0)
				command = modif_itoa(copy->value, input);
			else
				command = modif_strdup("tmp", input);
			copy->name = ft_strjoin("heredoc_", command, input);
			printf("name=%s\n", copy->name);
			free(command); //ft_change_limiter
			printf("2name=%s\n", copy->name);
		}
		copy = copy->next;
		printf("add_heredoc\n");
	}
	printf("111add_heredoc\n");
}
