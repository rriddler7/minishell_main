/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 20:16:58 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:53:22 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	launch_heredoc2(t_direct *copy, t_input *input, int *fd)
{
	*fd = open(copy->name, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0644);
	if (*fd == -1)
	{
		print_error(input, errno, copy->name, strerror(errno));
		return (-3);
	}
	return (*fd);
}

t_status	launch_heredoc3(t_direct *copy, t_input *input, char **err)
{
	free(*err);
	print_error(input, errno, copy->name, NULL);
	return (fail);
}

t_status	launch_heredoc(t_direct *copy, t_input *input, int fd)
{
	char		*new_line;
	char		*err;

	if (launch_heredoc2(copy, input, &fd) == -3)
		return (fail);
	while (1)
	{
		write(1, "> ", 2);
		my_gnl(&new_line, input);
		if (ft_strcmp(new_line, copy->stop_word) == 1)
		{
			err = ft_strjoin(new_line, "\n", input);
			if (write(fd, err, ft_strlen(err)) == -1)
				return (launch_heredoc3(copy, input, &err));
			free(err);
		}
		else
		{
			free(new_line);
			break ;
		}
		free(new_line);
	}
	close(fd);
	return (success);
}
