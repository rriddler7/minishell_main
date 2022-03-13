/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 19:27:20 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 01:13:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	create_new_env(t_env **new, DIR *dir, \
struct dirent *entry, t_input *input)
{
	*new = create_new_list(ft_strjoin_for_3 \
	(modif_itoa((int)entry->d_ino, input), "=", \
	modif_strdup(entry->d_name, input), input), input);
	(*new)->equal = 0;
	if (!input->star)
		input->star = *new;
	else
		add_list_back(new, &input->star);
}

void	find_star_in_comm(t_input *input)
{
	DIR				*dir;
	struct dirent	*entry;
	t_env			*new;

	dir = opendir(getcwd(NULL, 0));
	if (!dir)
		return ;
	while (1)
	{
		entry = readdir(dir);
		if (entry != NULL)
		{
			if (entry->d_name[0] != '.')
				create_new_env(&new, dir, entry, input);
		}
		else
			break ;
	}
	closedir(dir);
}

char	*change_star(t_comm **tmp, t_input *input, U_INT *k)
{
	t_env	*copy;
	t_env	*min;

	copy = input->star;
	if (*k == 0)
	{
		min = copy;
		while (copy)
		{
			if (modif_strncmp(copy->key, min->key, 1) > 0)
				min = copy;
			copy = copy->next;
		}
		(*tmp)->words[(*k)++] = modif_strdup(min->value, input);
		return (modif_strdup(min->value, input));
	}
	else
	{
		while (copy)
		{
			(*tmp)->words[(*k)++] = modif_strdup(copy->value, input);
			copy = copy->next;
		}
		return ((*tmp)->words[*k]);
	}
}

t_templ	*create_template_list(char *str, t_input *input, U_INT num, U_INT i)
{
	t_templ	*templ;
	t_templ	*copy;

	templ = NULL;
	while (str[i])
	{
		if (str[i] != '*')
			num++;
		while (str[i] && str[i] != '*')
			i++;
		while (str[i] && str[i] == '*')
			i++;
	}
	while (num--)
	{
		copy = malloc(sizeof(t_templ));
		copy->next = NULL;
		if (!templ)
			templ = copy;
		else
			lstadd_back(&templ, copy);
	}
	add_list_in_templ(templ, str, input, 0);
	return (templ);
}

U_INT	check_tmp_in_dir(t_templ *templ, t_input *input)
{
	t_status	flag;
	U_INT		len;
	t_env		*copy;
	U_INT		i;

	flag = 0;
	len = 0;
	i = 0;
	copy = input->star;
	while (copy)
	{
		if (check_template(input, copy->value, templ, 0) == success)
		{
			++len;
			copy->equal = 1;
		}
		copy = copy->next;
	}
	return (len);
}
