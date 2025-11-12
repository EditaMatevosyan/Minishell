/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:14:07 by edmatevo          #+#    #+#             */
/*   Updated: 2025/11/12 13:34:37 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_syntax_error(const char *token)
{
    fprintf(stderr, "minishell: syntax error: %s\n", token);
}

int operator_type(int type)
{
    return (type == T_PIPE || type == T_REDIR_IN || type == T_REDIR_OUT ||
            type == T_HEREDOC || type == T_APPEND);
}

static int check_edges(t_token *tok)
{
    t_token *last;

    if(!tok)
        return 0;
    if(tok->type == T_PIPE)
    {
        print_syntax_error("|");
        return 1;
    }
    last = tok;
    while(last->next)
        last = last->next;
    if(is_operator(last->type))
    {
        print_syntax_error("newline");
        return 1;
    }
    return 0;
}

int syntax_check(t_token *tok)
{
    if(check_edges(tok))
        return 1;
    while(tok && tok->next)
    {
        if(operator_type(tok->type) && operator_type(tok->next->type))
        {
            if(tok->type == T_PIPE)
                print_syntax_error("|");
            else
                print_syntax_error(tok->next->value);
            return 1;
        }
        if((tok->type == T_REDIR_IN || tok->type == T_REDIR_OUT ||
            tok->type == T_HEREDOC || tok->type == T_APPEND) && 
            (!tok->next || tok->next->type != T_WORD))
        {
            if(!tok->next)
                print_syntax_error("newline");
            else
                print_syntax_error(tok->next->value);   
            return 1;
        }
        tok = tok->next;
    }
    return 0;
}
