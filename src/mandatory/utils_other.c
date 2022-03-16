/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_other.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 15:58:39 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 00:02:33 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

U_INT	ft_strlen(char *s)
{
	U_INT	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

t_status	ft_strcmp(char *s1, char *s2)
{
	U_INT	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (fail);
		i++;
	}
	return (success);
}

inline static int	num_len(long num)
{
	int	len;

	len = 0;
	if (num < 0)
	{
		num = -num;
		len++;
	}
	while (num / 10 > 0)
	{
		len++;
		num = num / 10;
	}
	len++;
	return (len);
}

char	*modif_itoa(int n, t_input *input)
{
	char	sign;
	int		len;
	char	*str;
	long	num;

	num = n;
	len = num_len(num);
	if (n < 0)
	{
		sign = '-';
		num = -num;
	}
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		print_error(input, 12, "malloc", NULL);
	str[len--] = '\0';
	while ((num / 10) > 0)
	{
		str[len--] = (num % 10) + 48;
		num = num / 10;
	}
	str[len] = num + 48;
	if (sign == '-')
		str[len - 1] = '-';
	return (str);
}

long long	modif_atoi(char *s, int *flag, int i)
{
	int					sign;
	unsigned long long	number;

	number = 0;
	if (*flag != 0)
		return (0);
	if (s[i] == '-')
		sign = -1;
	else
		sign = 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i] != 0 && *flag == 0)
	{
		if (s[i] < '0' || s[i] > '9')
			*flag = -1;
		while (s[i] >= '0' && s[i] <= '9' && *flag == 0)
		{
			number = 10 * number + (s[i++] - '0');
			if ((number > 9223372036854775807 && sign == 1) || \
				(sign == -1 && number - 1 > 9223372036854775807))
				*flag = -1;
		}
	}
	return ((long long)sign * number);
}
