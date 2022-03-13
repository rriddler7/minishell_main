/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:20:45 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/13 19:43:15 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_input input;
    char *str_command;

    if (argc != 1)
    {
        printf("minishell: %s: %s\n", argv[1], strerror(2));
        printf("from main: TYPE ERROR %d\n", 127); //???? удалить!
        exit(127);//exit(1)??
    }
    init_input(&input);
    // printf("%d, %d \n", input.std_in, input.std_out);
    put_envp(envp, &input);
    put_shlvl(&input); // вызов шелла в шелле - пока не работает!
	while (1)
	{
		if (catch_str(&str_command, &input, 0) == fail)
			continue ;//считываем строку через readline
		if (finder(str_command, &input, 0) == fail) // здесь надо дописать обработку ошибок
		{
			free(str_command);
			continue ;
		}
		if (parser(str_command, &input) != success)
		{
			free(str_command);
			continue ;
		}
		clean_command(&input);
		// printf("0\n");
		clean_direct(&input);
		// printf("1\n");
		make_env_array(&input, &input.arg_env);
		// // printf("2\n");
		try_open(&input);
		if (str_command) //по идее тут еще будет очистка fd - void	free_str(t_arg *arg, char *str)
		{
			free(str_command);
			
			// free_env(&input);
			free_all(&input);
		}
	}
    return(0);
}