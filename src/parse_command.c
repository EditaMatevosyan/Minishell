/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:40:38 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 18:29:11 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_heredocs(t_cmd *cmd, int total)
{
	int	i;

	if (total <= 0)
		return (0);
	cmd->heredoc_delims = malloc(sizeof(char *) * total);
	cmd->heredoc_fds = malloc(sizeof(int) * total);
	cmd->heredoc_expands = malloc(sizeof(int) * total);
	if (!cmd->heredoc_delims || !cmd->heredoc_fds || !cmd->heredoc_expands)
		return (-1);
	i = 0;
	while (i < total)
	{
		cmd->heredoc_delims[i] = NULL;
		cmd->heredoc_fds[i] = -1;
		cmd->heredoc_expands[i] = 1;
		i++;
	}
	return (0);
}

static int	init_cmd_buffers(t_cmd *cmd, t_token *cur, int *arg_count)
{
	int	i;
	int	total_heredocs;

	*arg_count = count_args(cur);
	cmd->argv = malloc((*arg_count + 1) * sizeof(char *));
	if (!cmd->argv)
		return (-1);
	i = 0;
	while (i <= *arg_count)
		cmd->argv[i++] = NULL;
	total_heredocs = count_heredocs(cur);
	return (init_heredocs(cmd, total_heredocs));
}

static int	process_word_token(t_cmd *cmd, t_token **tok, t_env *env, int *argc)
{
	int		skip_arg;
	char	*arg;

	skip_arg = 0;
	arg = join_expanded_arg(tok, env, &skip_arg);
	if (!arg && !skip_arg)
		return (-1);
	if (skip_arg)
		return (0);
	cmd->argv[*argc] = arg;
	(*argc)++;
	return (0);
}

static int	process_token(t_cmd *cmd, t_token **tok, t_env *env, int *argc)
{
	if ((*tok)->type == T_WORD)
		return (process_word_token(cmd, tok, env, argc));
	if ((*tok)->type == T_REDIR_IN || (*tok)->type == T_REDIR_OUT
		|| (*tok)->type == T_APPEND || (*tok)->type == T_HEREDOC)
		return (handle_redirection(cmd, tok, env));
	*tok = (*tok)->next;
	return (0);
}

t_cmd	*parse_command(t_token **cur, t_env *env)
{
	t_cmd	*cmd;
	int		argc;
	t_token	*tok;
	int		arg_count;

	cmd = calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	if (init_cmd_buffers(cmd, *cur, &arg_count) == -1)
		return (free_cmd(cmd), NULL);
	argc = 0;
	tok = *cur;
	while (tok && tok->type != T_PIPE)
	{
		if (process_token(cmd, &tok, env, &argc) == -1)
			return (free_cmd(cmd), NULL);
	}
	cmd->argv[argc] = NULL;
	*cur = tok;
	return (cmd);
}
