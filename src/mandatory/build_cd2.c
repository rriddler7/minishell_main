/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 22:30:35 by rriddler          #+#    #+#             */
/*   Updated: 2022/03/16 18:37:59 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pwd_envp(t_input *input, char *path, char *key)
{
	char	*tmp;
	t_env	*new;

	tmp = ft_strjoin_for_3(key, "=", path, input);
	new = create_new_list(tmp, input);
	free(tmp);
	if (!new)
		cd_print_error(input, 12, "malloc", NULL);
	add_list_back(&new, input);
}

static void	change_envp(t_input *input, char *path, t_status flag)
{
	t_env	*copy;
	char	*tmp;
	t_env	*new;
	char	*key;

	path = getcwd(NULL, 0);
	if (!path)
		cd_print_error(input, errno, "getcwd", NULL);
	if (flag == 0)
		key = modif_strdup("OLDPWD", input);
	else
		key = modif_strdup("PWD", input);
	copy = input->envp;
	while (copy && ft_strcmp(copy->key, key) != success)
		copy = copy->next;
	if (!copy)
		pwd_envp(input, path, key);
	else
	{
		free(copy->value);
		copy->value = path;
	}
	free(key);
}

char	*cd_oldpwd(t_input *input, t_comm *command, t_env *copy)
{
	char	*path;
	char	*tmp;

	path = NULL;
	while (copy && ft_strcmp(copy->key, "OLDPWD"))
		copy = copy->next;
	if (copy)
	{
		tmp = modif_substr(command->words[1], 1, \
			ft_strlen(command->words[1]) - 1, input);
		path = ft_strjoin(copy->value, tmp, input);
		free(tmp);
		return (path);
	}
	return (0);
}

U_INT	launch_cd2(t_input *input, t_comm **command, char sign, char **path)
{
	if (sign == '-')
	{
		*path = cd_oldpwd(input, *command, input->envp);
		if (!*path)
		{
			write(2, "cd: OLDPWD not set\n", 17);
			return (1);
		}
		printf("%s\n", *path);
	}
	else
	{
		if (sign == '~')
			*path = path_home(input, *command);
		else
			*path = get_path_cd(input, *command, input->envp);
		if (!*path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	return (0);
}	

U_INT	launch_cd(t_input *input, t_comm *command, char	*path)
{
	t_env	*copy;

	copy = input->envp;
	if (!input || !command->words[1])
	{
		while (copy && ft_strcmp(copy->key, "HOME"))
			copy = copy->next;
		if (copy)
			path = modif_strdup(copy->value, input);
	}
	else
	{
		if (launch_cd2(input, &command, command->words[1][0], &path) == 1)
			return (1);
	}
	change_envp(input, NULL, 0);
	if (chdir(path) != success)
		cd_print_error(input, 1, command->words[0], \
		"No such file or directory");
	else
		change_envp(input, NULL, 1);
	input->num_error = 1;
	free(path);
	return (input->num_error);
}
