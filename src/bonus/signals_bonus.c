/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:00:13 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:57:16 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	main_handler(int signal)
{
	(void)signal;
	rl_on_new_line();
	rl_redisplay();
	write(2, "  \b\b\n", 5);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handler_pipex(int sig_num)
{
	if (sig_num == SIGINT)
		write(2, "\n", 1);
	if (sig_num == SIGQUIT)
		write(2, "Quit: 3\n", 8);
}

void	in_signal(t_input *input, U_INT i, t_comm *copy, U_INT counter)
{
	U_INT		len;
	t_status	flag;

	flag = 0;
	while (counter++ < i)
		copy = copy->next;
	if (copy->words == NULL || copy->words[0] == NULL)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	len = ft_strlen(copy->words[0]);
	if (ft_strlen(copy->words[0]) > 9 && ft_strcmp(&copy->words[0][len - 10], \
	"/minishell") == success)
		flag = 1;
	if (ft_strcmp(copy->words[0], "minishell") == fail && flag == 0)
	{
		signal(SIGINT, handler_pipex);
		signal(SIGQUIT, handler_pipex);
		return ;
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	handler_child(int sig_num)
{
	if (sig_num == SIGINT)
		exit(2);
	if (sig_num == SIGQUIT)
		exit(3);
}
