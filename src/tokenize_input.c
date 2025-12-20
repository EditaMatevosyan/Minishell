/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:11:20 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 15:30:36 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_glued_flag_for_last(t_token *head, int glued)
{
	if (!head)
		return ;
	while (head->next)
		head = head->next;
	head->glued = glued;
}

static int	add_quoted_token(t_minishell *shell, char **input_ptr, int *i,
		int had_space)
{
	char	*token;
	int		expand;
	int		quoted;

	expand = 0;
	quoted = 0;
	token = extract_quoted(input_ptr, i, &expand, &quoted);
	if (!token)
		return (fprintf(stderr,
				"minishell:syntax error: unexpected end of file\n"), -1);
	add_token(&shell->tokens, new_token(token, T_WORD, expand, quoted));
	set_glued_flag_for_last(shell->tokens, !had_space);
	return (0);
}

static int	dispatch_token(t_minishell *shell, char **input_ptr, int *i,
		int had_space)
{
	char	*token;

	if (is_operator((*input_ptr)[*i]))
	{
		if (handle_operator_token(shell, *input_ptr, i) == -1)
			return (-1);
		return (set_glued_flag_for_last(shell->tokens, !had_space), 0);
	}
	if (is_quote((*input_ptr)[*i]))
		return (add_quoted_token(shell, input_ptr, i, had_space));
	token = extract_word(*input_ptr, i);
	if (!token)
		return (-1);
	add_token(&shell->tokens, new_token(token, T_WORD, 1, 0));
	set_glued_flag_for_last(shell->tokens, !had_space);
	if (*input_ptr == NULL)
		return (-1);
	return (0);
}

static int	tokenize_loop(t_minishell *shell, char **input_ptr)
{
	int	i;
	int	had_space;

	i = 0;
	had_space = 1;
	while ((*input_ptr)[i])
	{
		had_space = 0;
		while (is_space((*input_ptr)[i]))
		{
			i++;
			had_space = 1;
		}
		if (!(*input_ptr)[i])
			break ;
		if (dispatch_token(shell, input_ptr, &i, had_space) == -1)
			return (-1);
	}
	return (0);
}

int	tokenize_input(t_minishell *shell, char **input_ptr)
{
	if (!shell || !input_ptr || !*input_ptr)
		return (-1);
	free_tokens(&shell->tokens);
	return (tokenize_loop(shell, input_ptr));
}
