/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 14:34:09 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/11 15:19:30 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_str(char *str, t_env *env)
{
    int     i = 0;
    char    *res = ft_strdup("");
    char    *tmp;

    if (!res)
        return (NULL);

    while (str[i])
    {
        if (str[i] == '$')
        {
            i++;
            if (!str[i] || (str[i] != '?' && !ft_isalnum(str[i]) && str[i] != '_'))
            {
                char lit[2] = {'$', '\0'};
                tmp = str_join_free(res, ft_strdup(lit));
                if (!tmp)
                    return (NULL);
                res = tmp;
                continue ;
            }

            if (str[i] == '?')
            {
                tmp = str_join_free(res, ft_itoa(g_exit_status));
                if (!tmp)
                    return (NULL);
                res = tmp;
                i++;
                continue ;
            }

            {
                int start = i;
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    i++;

                char *key = ft_substr(str, start, i - start);
                if (!key)
                    return (free(res), NULL);

                char *val = get_env_value(env, key);
                free(key);

                char *to_append = val ? ft_strdup(val) : ft_strdup("");
                if (!to_append)
                    return (free(res), NULL);

                tmp = str_join_free(res, to_append);
                if (!tmp)
                    return (NULL);
                res = tmp;
            }
        }
        else
        {
            char lit[2] = {str[i], '\0'};
            tmp = str_join_free(res, ft_strdup(lit));
            if (!tmp)
                return (NULL);
            res = tmp;
            i++;
        }
    }
    return (res);
}

/* returns a new malloc'd string */
static char *tilde(const char *s, t_env *env)
{
    const char *home;
    char       *res;

    if (!s || s[0] != '~')
        return (ft_strdup(s ? s : ""));
    if (s[1] && s[1] != '/')
        return (ft_strdup(s)); /* not handling ~user */
    home = get_env_value(env, "HOME");
    if (!home)
        return (ft_strdup(s)); /* bash leaves it as "~" if HOME is unset */
    /* join HOME + (s + 1) */
    res = ft_strjoin2(home, s + 1); /* ft_strjoin2 creates a new string and doesn't free inputs */
    return (res);
}

int expand_tokens(t_token *tok, t_env *env)
{
    while (tok)
    {
        if (tok->expand == 1 && tok->value)
        {
            char *pre;
            char *newv;

            if (tok->quoted == 0)
                pre = tilde(tok->value, env);
            else
                pre = ft_strdup(tok->value);
            if (!pre)
                return (-1);
            newv = expand_str(pre, env);                /* your existing $VAR expander */
            free(pre);
            if (!newv)
                return (-1);
            free(tok->value);
            tok->value = newv;
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
	int		i;

	if (!*tok)
    {
        fprintf(stderr, "minishell: syntax error near redirection\n");
        return -1;
    }
	append = ((*tok)->type == T_APPEND);
	type = (*tok)->type;
	*tok = (*tok)->next;
	
	//handle heredoc
	if (type == T_HEREDOC)
	{
		i = cmd->heredoc_count;
		if (!*tok || (*tok)->type != T_WORD)
		{
			fprintf(stderr, "minishell: syntax error near redirection\n");
			return (-1);
		}
		cmd->heredoc_delims[i] = ft_strdup((*tok)->value);
    	if (!cmd->heredoc_delims[i])
        	return (-1);
    	if ((*tok)->quoted == 1 || (*tok)->quoted == 2)
        	cmd->heredoc_expands[i] = 0;
    	else
        	cmd->heredoc_expands[i] = 1;
    	cmd->heredoc_count++;
    	*tok = (*tok)->next;
   		return (0);	
	}
	if (!*tok || (*tok)->type != T_WORD)
	{
		fprintf(stderr, "minishell: syntax error near redirection\n");
		return (-1);
	}
	if ((*tok)->expand == 1)
		value = expand_str((*tok)->value, env);
	else
		value = ft_strdup((*tok)->value);
    //debug
    printf("values %s\n", value);
	if (!value)
		return (-1);
	if (type == T_REDIR_IN)
	{
        
        //debug
        printf("enters type == T_REDIR_IN\n");
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
	else if (type == T_REDIR_OUT || type == T_APPEND)
	{
        //debug
        printf("enters type == T_REDIR_OUT\n");
		free(cmd->outfile);
		cmd->outfile = value;        
		cmd->append = append;
	}
	*tok = (*tok)->next;
	return (0);
}

int count_args(t_token *tok)
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
    int     argc;
    t_token *tok;
    int   arg_count;
	int		i;
	int		j;
	int		total_heredocs;

    cmd = calloc(1, sizeof(t_cmd));
    if (!cmd)
        return (NULL);

    arg_count = count_args(*cur);
    //debug
    printf("%d\n", arg_count);
	argc = 0;
    cmd->argv = malloc((arg_count + 1) * sizeof(char *));
    if (!cmd->argv)
        return (free(cmd), NULL);
	else
	{
		i = 0;
		while (i < arg_count)
		{
			cmd->argv[i] = NULL;
			i++;
		}
	}
	total_heredocs = count_heredocs(*cur);
	if (total_heredocs > 0)
	{
    	cmd->heredoc_delims   = malloc(sizeof(char *) * total_heredocs);
    	cmd->heredoc_fds      = malloc(sizeof(int) * total_heredocs);
    	cmd->heredoc_expands = malloc(sizeof(int) * total_heredocs);

    	if (!cmd->heredoc_delims || !cmd->heredoc_fds || !cmd->heredoc_expands)
		{
        	return (free_cmd(cmd), NULL);
		}
		j = 0;
		while (j < total_heredocs)
		{
    		cmd->heredoc_delims[j] = NULL;
        	cmd->heredoc_fds[j] = -1;
        	cmd->heredoc_expands[j] = 1;
			j++;
		}
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
            {
                //debug
                printf("redirection handling is not ok");
                return (free_cmd(cmd), NULL);
            }
            continue;
        }
        tok = tok->next;
    }
    cmd->argv[argc] = NULL;
    *cur = tok;
    return (cmd);
}


void free_cmd(t_cmd *cmd)
{
	int i;
	int	j;

    if (!cmd)
        return;
    if (cmd->argv)
    {
		i = 0;
       while (cmd->argv[i])
		{
			if (cmd->argv[i])
            	free(cmd->argv[i]);
			i++;
		}
        free(cmd->argv);
    }
	if (cmd->infile)
    	free(cmd->infile);
	if (cmd->outfile)
    	free(cmd->outfile);
	if (cmd->heredoc_delims)
	{
		j = 0;
		while (j < cmd->heredoc_count)
		{
			if (cmd->heredoc_delims[j])
				free(cmd->heredoc_delims[j]);
			j++;
		}
        free(cmd->heredoc_delims);
        cmd->heredoc_delims = NULL;
	}
    if (cmd->heredoc_fds)
    {
        free(cmd->heredoc_fds);
        cmd->heredoc_fds = NULL;
    }
    if (cmd->heredoc_expands)
    {
        free(cmd->heredoc_expands);
        cmd->heredoc_expands = NULL;
    }
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
