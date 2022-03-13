/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:14:27 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 01:19:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    init_input(t_input *input)
{
	input->num_of_command = 0;
	input->arg_env = NULL;
	input->fd = NULL;
	input->num_error = 0;
	input->problem = NULL;
    input->command = NULL;
	input->direct = NULL;
	input->envp = NULL;
	inpit->star = NULL;
}