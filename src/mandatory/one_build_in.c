/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_build_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 23:04:01 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/13 20:24:24 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

U_INT	launcher(t_input *input, t_comm	*command) //int	run_built(t_cmd *cmd, t_arg *arg)
{
	if (command->build_number == 1)
		return (launch_echo(input));
	else if (command->build_number == 2)
		return (launch_cd(input, command));
	else if (command->build_number == 3)
		return (launch_pwd(input));
	else if (command->build_number == 4)
		return (launch_export(input, command, 1));
	else if (command->build_number == 5)
		return (launch_unset(input, command));
	else if (command->build_number == 6)
		return (launch_env(input));
	else if (command->build_number == 7)
		return (launch_exit(input, command));
	return (success);
}

int	reverse_redir(t_input *input)
{
	int fd_safe;
	int	new_file; // новый файл для редиректа
	
	if (!input->command->direct_out)
		return (-1);
	if (input->command->direct_out->twin == 1) // это heredoc <<
		new_file = open(input->command->direct_out->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else //это <
		new_file = open(input->command->direct_out->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new_file == -1)
		print_error(input, errno, input->command->direct_out->name, NULL);
	dup2(STDOUT_FILENO, fd_safe); // fd_safe = STDOUT_FILENO
	if (fd_safe == -1)
	{
		close(new_file);
		print_error(input, errno, "dup2", NULL);
	}
	if (dup2(new_file, STDOUT_FILENO) == -1) // STDOUT_FILENO = new_file
	{
		close(new_file);
		close(fd_safe);
		print_error(input, errno, "dup2", NULL);
	}
	close(new_file);
	return(fd_safe);
}

void	reverse_redir2(t_input *input, int fd)
{
	if (fd == -1)
		return ;
	if (dup2(fd, STDOUT_FILENO) == -1) // STDOUT_FILENO = fd1 вернули станд.вывод
	{
		close(fd);
		print_error(input, errno, "dup2", NULL);
		return ;
	}
	close(fd);
}