/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 14:34:09 by edmatevo          #+#    #+#             */
/*   Updated: 2025/10/28 18:36:37 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_str(char *str, t_env *env)
{
    int i = 0;
    char *res = ft_strdup("");
    if (!res)
        return (NULL);

    while (str[i])
    {
        if (str[i] == '$')
        {
            i++;
            if (str[i] == '?') 
            {
                res = str_join_free(res, ft_itoa(g_exit_status));
                i++;
            }
            else                
            {
                int start = i;
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    i++;
                char *key = ft_substr(str, start, i - start);
                char *val = get_env_value(env, key);
                free(key);
                if (!val)
                    val = ft_strdup(""); 
                res = str_join_free(res, val); 
            }
        }
        else
        {
            char tmp[2];
            tmp[0] = str[i];
            tmp[1] = '\0';
            res = str_join_free(res, ft_strdup(tmp));
            i++;
        }
    }
    return (res);
}

int expand_tokens(t_token *tok, t_env *env)
{
    while (tok)
    {
        if (tok->expand == 1 && tok->value)
        {
            char *new = expand_str(tok->value, env);
            if (!new)
                return (-1);
            free(tok->value);
            tok->value = new;
        }
        tok = tok->next;
    }
    return (0);
}

t_cmd *parse_tokens(t_token *tokens, t_env *env)
{
    t_cmd *cmd_list = NULL;
    t_cmd *last = NULL;
    t_cmd *cmd;

    while (tokens)
    {
        cmd = parse_command(&tokens, env);
        if (tokens && tokens->type == T_PIPE)
            ; // pipe handled in parse_command
        if (!cmd)
        {
            free_cmd_list(&cmd_list);
            return (NULL);
        }
        if (!cmd_list)
            cmd_list = cmd;
        else
            last->next = cmd;
        last = cmd;
        if (tokens && tokens->type == T_PIPE)
            tokens = tokens->next; // skip '|'
    }
    return (cmd_list);
}

static char *join_expanded_arg(t_token **tok, t_env *env)
{
    char *arg = NULL;

    while (*tok && (*tok)->type == T_WORD)
    {
        char *part;
        if ((*tok)->quoted == 2) // single-quote
            part = ft_strdup((*tok)->value);
        else if ((*tok)->expand) // unquoted or double-quote
            part = expand_str((*tok)->value, env);
        else
            part = ft_strdup((*tok)->value);

        if (!part)
            return (free(arg), NULL);

        if (!arg)
            arg = part;
        else
        {
            char *joined = malloc(ft_strlen(arg) + ft_strlen(part) + 1);
            if (!joined)
                return (free(arg), free(part), NULL);
            ft_strcpy(joined, arg);
            ft_strcat(joined, part);
            free(arg);
            free(part);
            arg = joined;
        }

        // stop joining if next token is quoted differently
        if ((*tok)->next && (*tok)->next->quoted != (*tok)->quoted)
            break;

        *tok = (*tok)->next;
    }

    return arg;
}


static int handle_redirection(t_cmd *cmd, t_token **tok, t_env *env)
{
    int append = ((*tok)->type == T_APPEND);
    int type = (*tok)->type;
    *tok = (*tok)->next;
    if (!*tok || (*tok)->type != T_WORD)
        return (fprintf(stderr, "syntax error near redirection\n"), -1);

    char *value = ((*tok)->expand == 1)
        ? expand_str((*tok)->value, env)
        : ft_strdup((*tok)->value);
    if (!value)
        return (-1);

    if (type == T_REDIR_IN || type == T_HEREDOC)
        cmd->infile = value;
    else
    {
        cmd->outfile = value;
        cmd->append = append;
    }
    *tok = (*tok)->next;
    return (0);
}

t_cmd *parse_command(t_token **cur, t_env *env)
{
    t_cmd  *cmd;
    int     argc = 0;
    t_token *tok;

    cmd = calloc(1, sizeof(t_cmd));
    if (!cmd)
        return (NULL);

    /* safer: zero-initialize the argv array */
    cmd->argv = calloc(64, sizeof(char *));
    if (!cmd->argv)
        return (free(cmd), NULL);

    tok = *cur;
    while (tok && tok->type != T_PIPE)
    {
        if (tok->type == T_WORD)
        {
            char *arg = join_expanded_arg(&tok, env);
            if (!arg)
                return (free_cmd(cmd), NULL);
            cmd->argv[argc++] = arg;
            continue;
        }
        if (tok->type == T_REDIR_IN || tok->type == T_REDIR_OUT
            || tok->type == T_APPEND || tok->type == T_HEREDOC)
        {
            if (handle_redirection(cmd, &tok, env) == -1)
                return (free_cmd(cmd), NULL);
            continue;
        }
        tok = tok->next;
    }
    cmd->argv[argc] = NULL;

    /* advance the caller's token pointer to where parsing stopped */
    *cur = tok;
    return (cmd);
}


void free_cmd(t_cmd *cmd)
{
    if (!cmd)
        return;
    if (cmd->argv)
    {
        for (int i = 0; cmd->argv[i]; i++)
            free(cmd->argv[i]);
        free(cmd->argv);
    }
    free(cmd->infile);
    free(cmd->outfile);
    free(cmd);
}

void free_cmd_list(t_cmd **list)
{
    t_cmd *tmp;
    t_cmd *next;

    tmp = *list;
    while (tmp)
    {
        next = tmp->next;
        free_cmd(tmp);
        tmp = next;
    }
    *list = NULL;
}
