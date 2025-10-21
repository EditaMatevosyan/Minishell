/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:11:20 by edmatevo          #+#    #+#             */
/*   Updated: 2025/10/21 16:38:59 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int handle_operator_token(t_minishell *shell, char *input, int *i)
{
    char *token = extract_operator(input, i);
    if (!token)
        return -1;
    if (token[0] == '|')
        add_token(&shell->tokens, new_token(token, T_PIPE, 1));
    else if (token[0] == '<' && token[1] == '<')
        add_token(&shell->tokens, new_token(token, T_HEREDOC, 1));
    else if (token[0] == '>' && token[1] == '>')
        add_token(&shell->tokens, new_token(token, T_APPEND, 1));
    else if (token[0] == '<')
        add_token(&shell->tokens, new_token(token, T_REDIR_IN, 1));
    else if (token[0] == '>')
        add_token(&shell->tokens, new_token(token, T_REDIR_OUT, 1));
    return 0;
}

static void append_to_token(t_token **tokens, char *extra)
{
    t_token *last;
    char *joined;

    if( !tokens || !*tokens || !extra)
        return;
    last = *tokens;
    while(last->next)
        last = last->next;
    joined = malloc(ft_strlen(last->value) + ft_strlen(extra) + 1);
    if (!joined)
        return;
    ft_strcpy(joined, last->value);
    ft_strcat(joined, extra);
    free(last->value);
    last->value = joined;
}

static int handle_quote_token(t_minishell *shell, char *input, int *i)
{
    char *token;
    int expand = 0;
    int start = *i;

    token = extract_quoted(input, i, &expand);
    if (!token)
    {
        fprintf(stderr, "minishell: syntax error: missing closing quote\n");
        return -1;
    }
    if (start > 0 && !is_space(input[start - 1]) && shell->tokens)
    {
        append_to_token(&shell->tokens, token);
        free(token);
        return 0;
    }
    else
        add_token(&shell->tokens, new_token(token, T_WORD, expand));
    
    return 0;
}

static int handle_word_token(t_minishell *shell, char *input, int *i)
{
    char *token;
    int start = *i;

    token = extract_word(input, i);
    if (!token)
        return -1;

    if (start > 0 && !is_space(input[start - 1]) && shell->tokens)
        append_to_token(&shell->tokens, token);
    else
        add_token(&shell->tokens, new_token(token, T_WORD, 1));
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
    return 0;
}


