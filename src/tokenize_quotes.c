/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:26:22 by edmatevo          #+#    #+#             */
/*   Updated: 2025/10/02 17:39:46 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_closing_quote(char *input, int start, char quote)
{
	int	i;

	i = start + 1;
	while (input[i])
	{
		if (input[i] == quote)
			return (i - (start + 1));
		i++;
	}
	return (-1);
}

char	*extract_word(char *input, int *i)
{
	int	start;
	int	len;
	char	*token;

	start = *i;
	while (input[*i] && !is_space(input[*i])
		&& !is_quote(input[*i]) && !is_operator(input[*i]))
		(*i)++;
	len = *i - start;
	token = ft_strdup(input + start, len);
	return (token);
}

char	*extract_quoted(char *input, int *i, int *expand)
{
	char	quote;
	int		start;
	int		len;
	char	*token;

	(void)expand;

	quote = input[*i];
	start = *i + 1;
	len = find_closing_quote(input, *i, quote);
	if (len == -1)
		return (NULL); 
	token = ft_strdup(input + start, len);
	*i = start + len + 1; 
	return (token);
}

char	*extract_operator(char *input, int *i)
{
	char	*token;

	if ((input[*i] == '>' && input[*i + 1] == '>') ||
		(input[*i] == '<' && input[*i + 1] == '<'))
	{
		token = ft_strdup(input + *i, 2);
		*i += 2;
	}
	else
	{
		token = ft_strdup(input + *i, 1);
		(*i)++;
	}
	return (token);
}
