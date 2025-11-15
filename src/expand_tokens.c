/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosie <rosie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 14:34:09 by edmatevo          #+#    #+#             */
/*   Updated: 2025/11/15 17:44:57 by rosie            ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
        /*if (tokens && tokens->type == T_PIPE)
            ; // pipe handled in parse_command*/
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

static char *dup_or_expand_part(t_token *t, t_env *env)
{
    if (t->quoted == 2)         // single quotes: no expand
        return ft_strdup(t->value);
    if (t->expand)              // unquoted or double quotes with expand
        return expand_str(t->value, env);
    return ft_strdup(t->value);  // safety
}

static char *join_expanded_arg(t_token **tok, t_env *env)
{
    char *arg = NULL;
    t_token *t = *tok;

    while (t && t->type == T_WORD)
    {
        char *part = dup_or_expand_part(t, env);
        if (!part) { 
            free(arg); 
            return NULL; 
        }

        if (!arg) 
            arg = part;
        else {
            char *joined = malloc(ft_strlen(arg) + ft_strlen(part) + 1);
            if (!joined) { 
                free(arg); 
                free(part); 
                return NULL; }
            ft_strcpy(joined, arg);
            ft_strcat(joined, part);
            free(arg); free(part);
            arg = joined;
        }

        // consume current token
        t = t->next;

        if (!t || t->type != T_WORD || t->glued == 0)
            break;
    }

    *tok = t; // caller will continue from first unconsumed token
    return arg;
}


static int	handle_redirection(t_cmd *cmd, t_token **tok, t_env *env)
{
	char	*value;
	int		append;
	int		type;
	int		fd;

	append = ((*tok)->type == T_APPEND);
	type = (*tok)->type;
	*tok = (*tok)->next;
	if (!*tok || (*tok)->type != T_WORD)
	{
		fprintf(stderr, "minishell: syntax error near redirection\n");
		return (-1);
	}
	if ((*tok)->expand == 1)
		value = expand_str((*tok)->value, env);
	else
		value = ft_strdup((*tok)->value);
	if (!value)
		return (-1);
	if (type == T_REDIR_IN || type == T_HEREDOC)
	{
		fd = open(value, O_RDONLY);
		if (fd == -1)
		{
			perror(value);
			free(value);
			return (-1);
		}
		close(fd);
		free(cmd->infile);
		cmd->infile = value;
	}
	else
	{
		cmd->outfile = value;          
		cmd->append = append;
	}
	*tok = (*tok)->next;
	return (0);
}

// static int count_args(t_token *tok)
// {
//     int count = 0;
//     while (tok && tok->type != T_PIPE)
//     {
// 		if (tok->type == T_WORD)
//         {
//             count++;
//             while (tok && tok->type == T_WORD && tok->glued == 1)
//                 tok = tok->next;
//             if (tok)
//                 tok = tok->next;
//         }
//         else
//             tok = tok->next;
//     }
//     return count;
// }

static int count_args(t_token *tok)
{
    int count = 0;
    while (tok && tok->type != T_PIPE)
    {
        if (tok->type == T_WORD)
            count++;
        tok = tok->next;
    }
    return count;
}


t_cmd *parse_command(t_token **cur, t_env *env)
{
    t_cmd  *cmd;
    int     argc = 0;
    t_token *tok;
    int   arg_count;

    cmd = calloc(1, sizeof(t_cmd));
    if (!cmd)
        return (NULL);

    arg_count = count_args(*cur);
	//debug
	printf("arg_count: %d\n", arg_count);
    cmd->argv = malloc((arg_count + 1) * sizeof(char *));
    if (!cmd->argv)
        return (free(cmd), NULL);
	else
	{
		for (int i = 0; i <= arg_count; i++)
    	cmd->argv[i] = NULL;
	}

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
		{
			if (cmd->argv[i])
            	free(cmd->argv[i]);
		}
        free(cmd->argv);
    }
	if (cmd->infile)
    	free(cmd->infile);
	if (cmd->outfile)
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
