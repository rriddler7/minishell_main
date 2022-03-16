/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:58 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:48:35 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	cd_print_error(t_input *input, U_INT \
	error_num, char *msg, char *descrip)
{
	input->num_error = error_num;
	write(2, "minishell: ", 11);
	if (msg != NULL)
	{
		write(2, msg, ft_strlen(msg));
		write(2, ": ", 2);
	}
	if (descrip == NULL)
		write(2, strerror(error_num), ft_strlen(strerror(error_num)));
	else
		write(2, descrip, ft_strlen(descrip));
	write(2, "\n", 1);
	free_all(input);
}

char	*get_path_cd(t_input *input, t_comm *command, t_env *copy)
{
	char	*path;
	char	*tmp;

	path = NULL;
	tmp = NULL;
	if (command->words[1][0] != '~')
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
			cd_print_error(input, errno, "getcwd", NULL);
		path = ft_strjoin_for_3(tmp, "/", command->words[1], input);
		free(tmp);
		return (path);
	}
	while (copy && ft_strcmp(copy->key, "HOME"))
		copy = copy->next;
	if (copy)
	{
		tmp = modif_substr(command->words[1], 1, \
			ft_strlen(command->words[1]) - 1, input);
		path = ft_strjoin(copy->value, tmp, input);
		free(tmp);
	}
	return (0);
}
