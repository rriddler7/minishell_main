/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 19:19:01 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 00:01:48 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**easy_realloc2(char **old, t_input *input, U_INT *size_good_temp)
{
	char	**res;
	U_INT	k;

	k = 0;
	while (old[k])
		++k;
	res = malloc(sizeof(char *) * (k + *size_good_temp + 1));
	if (!res)
		print_error(input, 12, "malloc", NULL);
	*size_good_temp = 0;
	return (res);
}

char	**easy(char **old, t_input *input, U_INT k, t_env *star)
{
	char	**res;
	U_INT	j;

	j = 0;
	res = easy_realloc2(old, input, &k);
	while (old[k])
	{
		if ((old[k] && !ft_strchr(old[k], '*')) || (old[k] && !n_star(old[k])))
		{
			res[j++] = modif_strdup(old[k], input);
			free(old[k++]);
		}
		else if ((old[k] && ft_strchr(old[k], '*')) && (n_star(old[k]) == 1))
		{
			while (star)
			{
				res[j++] = modif_strdup(star->value, input);
				star = star->next;
			}
			++k;
		}
	}
	res[j] = NULL;
	free(old);
	return (res);
}

void	realloc_words2(char **addition, char **res, t_input *input, U_INT *j)
{
	U_INT	n;

	n = 0;
	while (addition[n])
	{
		res[(*j)++] = modif_strdup(addition[n], input);
		free(addition[n++]);
	}
}

void	realloc_words3(char ***res, U_INT *j, char **oldi, t_input *input)
{
	(*res)[(*j)++] = modif_strdup(*oldi, input);
	free(*oldi);
}

char	**realloc_words(char **old, t_input *input, U_INT size, t_templ *templ)
{
	char		**res;
	U_INT		i;
	U_INT		j;
	char		**addition;

	j = 0;
	i = size;
	res = easy_realloc2(old, input, &i);
	while (old[i] && ft_strchr(old[i], '*') == NULL)
		realloc_words3(&res, &j, &old[i++], input);
	if (old[i] && ft_strchr(old[i], '*') != NULL)
	{
		addition = big_circle(templ, input, size);
		realloc_words2(addition, res, input, &j);
		free(addition);
		free(old[i++]);
	}
	while (old[i])
		realloc_words3(&res, &j, &old[i++], input);
	res[j] = NULL;
	free(old);
	return (res);
}
