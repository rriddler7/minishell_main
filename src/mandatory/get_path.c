/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 20:41:45 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 23:34:52 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path2(char *bin, t_input *input, U_INT i)
{
	char	*res;
	char	**path;
	U_INT	counter;

	path = ft_split(input->arg_env[i] + 5, ':');
	if (path == NULL)
		print_error(input, 12, "malloc", NULL);
	counter = 0;
	res = NULL;
	while (path[counter] != NULL && res == NULL)
	{
		res = ft_strjoin_for_3(path[counter], "/", bin, input);
		if (access(res, F_OK))
		{
			free(res);
			res = NULL;
		}
		counter++;
	}
	if (res == NULL)
		res = bin;
	clean_path(path, 0); //cmd_path
	// free(bin); //cmd_path+
	return (res);
}

char	*get_path(char *bin, t_input *input, U_INT i)
{
	char	*path;

	if (!access(bin, F_OK))
		return (bin);
	while (input->arg_env[i] != NULL && ft_strncmp(input->arg_env[i], \
	"PATH=", 5) != 0)
		i++;
	if (input->arg_env[i] == NULL)
	{
		access(bin, F_OK);
		print_error(input, errno, "bin", NULL);
	}
	return (get_path2(bin, input, i));
}
