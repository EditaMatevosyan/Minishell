/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_ut4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:12:42 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 15:29:00 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

const char	*skip_spaces(const char *str)
{
	while (*str && is_space(*str))
		str++;
	return (str);
}

int	get_sign(const char **str)
{
	const char	*s;
	int			sign;

	s = *str;
	sign = 1;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	*str = s;
	return (sign);
}

int	ft_atoll(const char *s, long long *out)
{
	long long	acc;
	int			sign;

	if (!s || !out)
		return (0);
	s = skip_spaces(s);
	sign = get_sign(&s);
	if (!acc_digits(&s, sign, &acc))
		return (0);
	s = skip_spaces(s);
	if (*s != '\0')
		return (0);
	if (sign == 1)
		*out = acc;
	else
		*out = acc * sign;
	return (1);
}
