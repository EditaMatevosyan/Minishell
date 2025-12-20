/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:26:22 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 19:21:01 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	count_open_quotes(char *input, int *single, int *doubleq)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && *doubleq == 0)
			*single = !*single;
		else if (input[i] == '"' && *single == 0)
			*doubleq = !*doubleq;
		i++;
	}
}

static int	append_line(char **input, char *line)
{
	char	*tmp;
	char	*new_input;

	tmp = ft_strjoin(*input, "\n");
	if (!tmp)
		return (free(line), 1);
	new_input = ft_strjoin(tmp, line);
	free(tmp);
	free(line);
	free(*input);
	if (!new_input)
		return (1);
	*input = new_input;
	return (0);
}

static int	read_until_quote(char **input, char quote)
{
	char	*line;
	int		single;
	int		doubleq;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (handle_unexpected_eof(input, quote));
		if (append_line(input, line))
			return (free(*input), *input = NULL, -1);
		single = 0;
		doubleq = 0;
		count_open_quotes(*input, &single, &doubleq);
		if ((quote == '\'' && single % 2 == 0) || (quote == '"' && doubleq
				% 2 == 0))
			break ;
	}
	return (0);
}

static int	resolve_quote_len(char **input_ptr, int *i, char quote)
{
	int	len;

	len = find_closing_quote(*input_ptr, *i, quote);
	if (len != -1)
		return (len);
	if (read_until_quote(input_ptr, quote) == -1 || !*input_ptr)
		return (-1);
	return (find_closing_quote(*input_ptr, *i, quote));
}

char	*extract_quoted(char **input_ptr, int *i, int *expand, int *quoted)
{
	char	quote;
	int		start;
	int		len;
	char	*token;

	if (!input_ptr || !*input_ptr)
		return (NULL);
	quote = (*input_ptr)[*i];
	if (quote == '"')
	{
		*quoted = 1;
		*expand = 1;
	}
	else
	{
		*quoted = 2;
		*expand = 0;
	}
	start = *i + 1;
	len = resolve_quote_len(input_ptr, i, quote);
	if (len == -1)
		return (NULL);
	token = ft_strndup((*input_ptr) + start, len);
	*i = start + len + 1;
	return (token);
}
