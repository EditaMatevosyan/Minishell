/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 17:55:59 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 18:09:19 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	join_and_set(char **res, char *to_append)
{
	char	*joined;

	if (!to_append)
	{
		free(*res);
		*res = NULL;
		return (-1);
	}
	joined = str_join_free(*res, to_append);
	if (!joined)
	{
		*res = NULL;
		return (-1);
	}
	*res = joined;
	return (0);
}

char	*dup_char(char c)
{
	char	lit[2];

	lit[0] = c;
	lit[1] = '\0';
	return (ft_strdup(lit));
}

int	append_literal_char(char **res, char c)
{
	char	*lit_dup;

	lit_dup = dup_char(c);
	return (join_and_set(res, lit_dup));
}

int	append_exit_status(char **res, int *i)
{
	char	*exit_str;

	exit_str = ft_itoa(g_exit_status);
	if (join_and_set(res, exit_str) == -1)
		return (-1);
	(*i)++;
	return (0);
}
