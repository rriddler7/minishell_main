/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:20:45 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 17:20:52 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline t_status	catch_str(char **command, t_input *input, U_INT i)
{
	*command = readline("minishell-1.0$ ");
	if (*command == NULL)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		write(2, "exit\n", 5);
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
	printf("main: incoming=%d, twin=%d\n", input->direct->incoming, input->direct->twin);
	clean_command(input);
	clean_direct(input);
	// printf("2main: incoming=%d, twin=%d\n", input->direct->incoming, input->direct->twin);
	make_env_array(input, &input->arg_env);
	try_open(input);
}

int	main(int argc, char **argv, char **envp)
{
	t_input		input;
	char		*str_command;

	rl_outstream = stderr; //test!!!!
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
