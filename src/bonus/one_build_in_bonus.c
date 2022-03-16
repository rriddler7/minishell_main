/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_build_in_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 23:04:01 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:54:00 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

U_INT	launcher(t_input *input, t_comm	*command)
{
	if (command->build_number == 1)
		return (launch_echo(input));
	else if (command->build_number == 2)
		return (launch_cd(input, command, NULL));
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
