/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cut_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 15:58:30 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 19:59:03 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*ft_strjoin(char *s1, char *s2, t_input *input)
{
	char	*newstr;
	U_INT	i;
	U_INT	j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	newstr = (char *)malloc(sizeof(char) * ((ft_strlen(s1) + \
					ft_strlen(s2)) + 1));
	if (!newstr)
		print_error(input, 12, "malloc", NULL);
	while (s1[i] != '\0')
	{
		newstr[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		newstr[i] = s2[j];
		i++;
		j++;
	}
	newstr[i] = '\0';
	return (newstr);
}

char	*ft_strjoin_for_3(char *s1, char *s2, char *s3, t_input *input)
{
	char	*first;
	char	*second;

	first = ft_strjoin(s1, s2, input);
	second = ft_strjoin(first, s3, input);
	free(first);
	return (second);
}

char	*modif_strdup(char *src, t_input *input)
{
	char	*dst;
	U_INT	i;

	i = 0;
	dst = (malloc(sizeof(char) * (ft_strlen(src) + 1)));
	if (dst == NULL)
		print_error(input, 12, "malloc", NULL);
	while (src[i] != 0)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*modif_substr(char *s, U_INT start, U_INT len, t_input *input)
{
	char	*sub;
	U_INT	i;

	if (!s)
		return (NULL);
	sub = malloc(sizeof(char) * (len + 1));
	if (!sub)
		print_error(input, 12, "malloc", NULL);
	i = 0;
	while (s[start + i] && (i < len))
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
