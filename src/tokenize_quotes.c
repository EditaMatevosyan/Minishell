/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:26:22 by edmatevo          #+#    #+#             */
/*   Updated: 2025/11/12 13:03:16 by edmatevo         ###   ########.fr       */
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
	if(*i == start)
		return (NULL);
	len = *i - start;
	token = ft_strndup(input + start, len);
	return (token);
}

static char *read_until_quote(char *input, char quote)
{
	char *line;
	char *tmp;

	while(1)
	{
		line = readline("> ");
		if(!line)
		{
			fprintf(stderr,
				"minishell: unexpected EOF while looking for matching `%c`\n",
				quote);
			free(input);
			return (NULL);
		}
		tmp = ft_strjoin(input, "\n");
		free(input);
		input = ft_strjoin(tmp, line);
		free(tmp);
		free(line);

		int single = 0;
		int doubleq = 0;
		int i = 0;
		while (input[i])
		{
			if (input[i] == '\'' && doubleq == 0)
				single = !single;
			else if (input[i] == '"' && single == 0)
				doubleq = !doubleq;
			i++;
		}
		if((quote == '\'' && single % 2 == 0) ||
		   (quote == '"' && doubleq % 2 == 0))
			break;
	}
	return (input);
}

char	*extract_quoted(char *input, int *i, int *expand, int *quoted)
{
	char	quote;
	int		start;
	int		len;
	char	*token;


	quote = input[*i];
	if (quote == '"')
	{
    	*quoted = 1;   // double-quote
    	*expand = 1;   // allow expansion
	}
	else
	{
		*quoted = 2;
		*expand = 0;  
	}
	start = *i + 1;
	len = find_closing_quote(input, *i, quote);
	if (len == -1)
	{
		input = read_until_quote(input, quote);
		if (!input)
			return (NULL);
		len = find_closing_quote(input, *i, quote);
		if (len == -1)
			return (NULL);
	}
	token = ft_strndup(input + start, len);
	*i = start + len + 1; 
	return (token);
}

char	*extract_operator(char *input, int *i)
{
	char	*token;

	if ((input[*i] == '>' && input[*i + 1] == '>') ||
		(input[*i] == '<' && input[*i + 1] == '<'))
	{
		token = ft_strndup(input + *i, 2);
		*i += 2;
	}
	else
	{
		token = ft_strndup(input + *i, 1);
		(*i)++;
	}
	return (token);
}

