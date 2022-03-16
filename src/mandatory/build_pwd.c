/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 14:29:10 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 23:20:26 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

U_INT	launch_pwd(t_input *input)
{
	char	*pwd;
	t_env	*copy;

	copy = input->envp;
	while (copy && ft_strcmp(copy->key, "PWD") != success)
		copy = copy->next;
	if (copy)
	{
		printf("%s\n", copy->value);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		print_error(input, errno, "getcwd", NULL);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
