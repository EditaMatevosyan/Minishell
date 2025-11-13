/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosie <rosie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:38:20 by edmatevo          #+#    #+#             */
/*   Updated: 2025/11/13 15:15:54 by rosie            ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

t_token *new_token(char *value, t_token_type type, int expand, int quoted)
{
    t_token *tok = malloc(sizeof(t_token));
    if (!tok) { free(value); return NULL; }
    tok->value = value;
    tok->type = type;
    tok->expand = expand;
    tok->quoted = quoted;
    tok->glued = 0;          // new
    tok->next = NULL;
    return tok;
}


void add_token(t_token **list, t_token *new)
{
    t_token *tmp;
    if(!new)
        return;
    if(!*list)
    {
        *list = new;
        return;
    }
    tmp = *list;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;    
}


void free_tokens(t_token **list)
{
    t_token *tmp;
    t_token *n;
    
    if(!list || !*list)
        return;
    tmp = *list;
    while(tmp)
    {
        n = tmp->next;
        free(tmp->value);
        free(tmp);
        tmp = n;
    }
    *list = NULL;
}
