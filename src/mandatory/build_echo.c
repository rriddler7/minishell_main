/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 17:57:00 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/13 20:21:45 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*path_home(t_input *input, t_comm *command)
{
	char	*path;
	char	*tmp;
	t_env	*copy;

	copy = input->envp;
	path = NULL;
	while (copy && ft_strcmp(copy->key, "HOME"))
		copy = copy->next;
	if (copy)
	{
		tmp = modif_substr(command->words[1], 1, \
			ft_strlen(command->words[1]) - 1, input);
		path = ft_strjoin(copy->value, tmp, input);
		free(tmp);
		return (path);
	}
	return (0);
}

static inline void	find_options(t_comm *command, U_INT *i, t_input *input)
{
	U_INT	w;
	U_INT	l;

	w = 1;
	while (command->words[w])
	{
		if (command->words[w][0] != '-')
			break ;
		if (command->words[w][0] == '~')
			printf("%s\n", path_home(input, command));
		l = 1;
		while (command->words[w][l] == 'n')
			l++;
		if (command->words[w][l])
			break ;
		free(command->words[w]);
		command->words[w] = modif_strdup("-n", input);
		w++;
	}
	while (command->words[*i] && ft_strcmp(command->words[*i], "-n") == 0)
		(*i)++;
}

U_INT	launch_echo(t_input *input)
{
	U_INT	i;
	U_INT	j;

	i = 1;
	find_options(input->command, &i, input);
	j = i - 1;
	while (input->command->words[i])
	{
		if (input->command->words[i + 1])
			printf("%s ", input->command->words[i]);
		else
		{
			if (input->command->words[i][0] != '~')
				printf("%s", input->command->words[i]);
		}
		i++;
	}
	if (j == 0)
		printf("\n");
	return (0);
}
