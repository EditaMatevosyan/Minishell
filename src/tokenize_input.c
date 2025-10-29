/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:11:20 by edmatevo          #+#    #+#             */
/*   Updated: 2025/10/28 17:33:12 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int handle_operator_token(t_minishell *shell, char *input, int *i)
{
    char *token = extract_operator(input, i);
    if (!token)
        return -1;
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
    return 0;
}

static int handle_quote_token(t_minishell *shell, char *input, int *i)
{
    char *token;
    int expand, quoted = 0;

    token = extract_quoted(input, i, &expand, &quoted);
    if (!token)
    {
        fprintf(stderr, "minishell: syntax error: missing closing quote\n");
        return -1;
    }
    add_token(&shell->tokens, new_token(token, T_WORD, expand, quoted));
    return 0;
}

static int handle_word_token(t_minishell *shell, char *input, int *i)
{
    char *token;

    token = extract_word(input, i);
    if (!token)
        return -1;
    add_token(&shell->tokens, new_token(token, T_WORD, 1, 0));
    return 0;
}

int tokenize_input(t_minishell *shell, char *input)
{
    int i = 0;

    if (!shell || !input)
        return -1;
    free_tokens(&shell->tokens);

    while (input[i])
    {
        while (is_space(input[i]))
            i++;
        if (!input[i])
            break;
        if (is_operator(input[i]))
        {
            if (handle_operator_token(shell, input, &i) == -1)
                return -1;
            continue;
        }
        else if (is_quote(input[i]))     
        {
            if (handle_quote_token(shell, input, &i) == -1)
                return -1;
            continue;
        }
        else 
        {
            if (handle_word_token(shell, input, &i) == -1)
                return -1;  
            continue;
        }
    }
    if (expand_tokens(shell->tokens, shell->env) == -1)
        return (-1);
    return 0;
}


