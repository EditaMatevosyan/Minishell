/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_basic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 15:20:00 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 19:20:55 by edmatevo         ###   ########.fr       */
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
	int		start;
	int		len;
	char	*token;

	start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_quote(input[*i])
		&& !is_operator(input[*i]))
		(*i)++;
	if (*i == start)
		return (NULL);
	len = *i - start;
	token = ft_strndup(input + start, len);
	return (token);
}

char	*extract_operator(char *input, int *i)
{
	char	*token;

	if ((input[*i] == '>' && input[*i + 1] == '>') || (input[*i] == '<'
			&& input[*i + 1] == '<'))
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

int	handle_operator_token(t_minishell *shell, char *input, int *i)
{
	char	*token;

	token = extract_operator(input, i);
	if (!token)
		return (-1);
	if (token[0] == '|')
		add_token(&shell->tokens, new_token(token, T_PIPE, 1, 0));
	else if (token[0] == '<' && token[1] == '<')
		add_token(&shell->tokens, new_token(token, T_HEREDOC, 1, 0));
	else if (token[0] == '>' && token[1] == '>')
		add_token(&shell->tokens, new_token(token, T_APPEND, 1, 0));
	else if (token[0] == '<')
		add_token(&shell->tokens, new_token(token, T_REDIR_IN, 1, 0));
	else if (token[0] == '>')
		add_token(&shell->tokens, new_token(token, T_REDIR_OUT, 1, 0));
	return (0);
}

int	handle_unexpected_eof(char **input, char quote)
{
	ft_putstr_fd("minishell: unexpected EOF near `", 2);
	ft_putchar_fd(quote, 2);
	ft_putstr_fd("`\n", 2);
	free(*input);
	*input = NULL;
	return (-1);
}
