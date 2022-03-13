/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 23:26:00 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/13 19:33:03 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**big_circle(t_templ *temple, t_input *input, U_INT size_good_temp)
{
	char	**addition;
	t_env	*copy;
	U_INT	i;

	i = 0;
	addition = malloc(sizeof(char *) * (size_good_temp) + 1);
	if (!addition)
		print_error(input, 12, "malloc", NULL);
	copy = input->star;
	while (copy)
	{
		if (copy->equal == 1)
		{
			addition[i] = modif_strdup(copy->value, input);
			copy->equal = 0;
			++i;
		}
		copy = copy->next;
	}
	addition[i] = NULL;
	return (addition);
}

void	put_one_in_equal(t_input *input)
{
	t_env	*copy;

	copy = input->star;
	while (copy)
	{
		copy->equal = 1;
		copy = copy->next;
	}
}

void	put_nol_in_equal(t_input *input)
{
	t_env	*copy;

	copy = input->star;
	while (copy)
	{
		copy->equal = 0;
		copy = copy->next;
	}
}

void	find_star2(t_comm **tmp, t_input *input, U_INT *i, t_templ	**templ)
{
	U_INT	len;

	len = check_tmp_in_dir(*templ, input);
	if (len)
	{
		(*tmp)->words = realloc_words((*tmp)->words, input, len, *templ);
		*i = 0;
	}
	else
		++(*i);
}

void	find_star(t_comm *tmp, t_input *in)
{
	U_INT	i;
	t_templ	*templ;

	i = 0;
	while (tmp && tmp->words && tmp->words[i])
	{	
		if (ft_strchr(tmp->words[i], '*') != NULL && n_star(tmp->words[i]) == 0)
		{
			templ = create_template_list(tmp->words[i], in, 0, 0);
			find_star2(&tmp, in, &i, &templ);
			put_nol_in_equal(in);
		}
		else if (ft_strchr(tmp->words[i], '*') && n_star(tmp->words[i]) == 1)
		{
			put_one_in_equal(in);
			tmp->words = easy(tmp->words, in, ft_lstsize(in->star), in->star);
			i = 0;
			put_nol_in_equal(in);
		}
		else if (ft_strchr(tmp->words[i], '*') == NULL)
			++i;
	}
}
