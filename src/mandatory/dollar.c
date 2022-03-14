/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 19:21:26 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 22:31:25 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*once_more_find_dollar(char *mid, t_input *input, U_INT i)
{
	while (mid[i])
	{
		if (mid[i] == '$')
			mid = find_dollar(mid, &i, input);
		else
			i++;
	}
	return (mid);
}

char	*find_dollar(char *tmp, U_INT *i, t_input *input)
{
	U_INT	j;
	char	*mid;

	mid = modif_strdup(tmp, input);
	free(tmp);
	j = *i + 1;
	if (mid[j] && mid[j] == '?')
		mid = find_question(mid, i, input, '\0');
	else if (mid[j] && mid[j] != '>' && mid[j] != '<' \
		&& mid[j] != '|' && mid[j] != ' ' && mid[j] != '$' \
		&& mid[j] != '\'' && mid[j] != '\"')
		mid = find_envp(mid, i, input);
	else
		(*i)++;
	return (mid);
}

char	*cut_envp(char *mid, U_INT *i, t_input *input)
{
	U_INT	j;
	char	*copy;
	t_env	*en;

	j = (*i)++;
	while (mid[*i] && mid[*i] != '>' && mid[*i] != '<' \
		&& mid[*i] != '|' && mid[*i] != ' ' && mid[*i] != '$' \
		&& mid[*i] != '\'' && mid[*i] != '\"')
		(*i)++;
	copy = modif_substr(mid, j + 1, *i - 1 - j, input);
	en = input->envp;
	while (en)
	{
		if (ft_strcmp(copy, en->key) == 0)
		{
			free(copy); //free+
			copy = modif_strdup(en->value, input);
			return (copy);
		}
		en = en->next;
	}
	free(copy);
	return (modif_strdup("", input));
}

char	*find_envp(char *mid, U_INT *i, t_input *input)
{
	U_INT	j;
	char	*envp;
	char	*copy;
	char	*end;

	j = *i + 1;
	copy = modif_substr(mid, 0, j - 1, input);
	while (mid[j] && mid[j] != '>' && mid[j] != '<' \
		&& mid[j] != '|' && mid[j] != ' ' && mid[j] != '$' \
		&& mid[j] != '\'' && mid[j] != '\"')
		j++;
	envp = cut_envp(mid, i, input);
	end = ft_strjoin(copy, envp, input);
	*i = ft_strlen(end);
	free(copy);
	copy = modif_substr(mid, j, ft_strlen(mid) - j, input);
	free(mid);
	mid = ft_strjoin(end, copy, input);
	free(end);
	free(copy);
	free(envp);
	return (mid);
}

char	*find_question(char *mid, U_INT *i, t_input *input, char c)
{
	U_INT	j;
	char	*start;
	char	*err;
	char	*end;

	j = *i + 1;
	start = modif_substr(mid, 0, j - 1, input);
	err = modif_itoa(input->num_error, input);
	end = ft_strjoin(start, err, input);
	*i = ft_strlen(end);
	free(start); //if_question
	j++;
	start = modif_substr(mid, j, ft_strlen(mid) - j, input);
	free(mid); //if_question
	mid = ft_strjoin(end, start, input);
	free(start); //if_question->ft_free
	free(end); //if_question->ft_free
	free(err); //if_question->ft_free
	return (mid);
}
