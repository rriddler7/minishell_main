/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:20:45 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:43:46 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline t_status	catch_str(char **command, t_input *input, U_INT i)
{
	*command = readline("\x1b[32mminishell$\x1b[0m ");
	if (*command == NULL)
	{
		write(2, "\033[Aminishell$ exit\n", \
		ft_strlen("\033[Aminishell$ exit\n"));
		rl_redisplay();
		free_all(input);
		exit(0);
	}
	if (*command && *command[0])
		add_history(*command);
	while ((*command)[i] == ' ')
		i++;
	if ((*command)[i] == '\0')
	{
		free(*command);
		return (fail);
	}
	return (success);
}

static inline void	go_parse_and_open(char *str_command, \
t_input *input, char **envp)
{
	if (parser(str_command, input, 0, 0) != success)
		return ;
	clean_command(input);
	clean_direct(input);
	make_env_array(input, &input->arg_env);
	try_open(input);
}

t_status	put_envp(char **envp, t_input *input)
{
	U_INT	i;
	t_env	*new;

	i = 0;
	while (envp[i])
	{
		new = create_new_list(envp[i], input);
		if (!new)
			print_error(input, 12, "malloc", NULL);
		add_list_back(&new, input);
		i++;
	}
	return (success);
}

int	main(int argc, char **argv, char **envp)
{
	t_input		input;
	char		*str_command;

	if (argc != 1)
	{
		printf("minishell-1.0: %s: %s\n", argv[1], strerror(2));
		exit(127);
	}
	init_input(&input);
	put_envp(envp, &input);
	put_shlvl(&input);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, main_handler);
		if (catch_str(&str_command, &input, 0) == fail)
			continue ;
		if (finder(str_command, &input, 0) != fail)
			go_parse_and_open(str_command, &input, envp);
		free_str_command(str_command, &input, 0);
	}
	return (0);
}
