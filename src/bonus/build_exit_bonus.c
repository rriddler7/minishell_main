/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exit_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:32 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 20:00:54 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

U_INT	launch_exit2(t_input *input, t_comm *command, int flag)
{
	char		*str;

	if (flag == -1)
	{
		if (input->num_of_command == 1)
			write(2, "exit\n", 5);
		str = ft_strjoin_for_3("minishell: exit: ", \
			command->words[1], ": numeric argument required\n", input);
		write(2, str, ft_strlen(str));
		free(str);
		input->num_error = 255;
		exit(255);
	}
	else if (command->words[2])
	{
		if (input->num_of_command == 1)
			write(2, "exit\n", 5);
		str = ft_strjoin("minishell: exit: ", "too many arguments\n", input);
		write(2, str, ft_strlen(str));
		free(str);
		input->num_error = 1;
		return (1);
	}
	return (0);
}

U_INT	launch_exit(t_input *input, t_comm *command)
{
	int			flag;
	long long	status;

	flag = 0;
	status = 0;
	if (command && command->words && command->words[1])
	{
		status = modif_atoi(command->words[1], &flag, 0);
		if (launch_exit2(input, command, flag) == 1)
			return (1);
		else
		{
			input->num_error = status;
			if (input->num_of_command == 1)
				write(2, "exit\n", 5);
			exit (status);
		}
	}
	else
	{
		input->num_error = status;
		write(2, "exit\n", 5);
		exit (status);
	}
	return (0);
}
