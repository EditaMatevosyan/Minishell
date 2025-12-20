/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:40:38 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 18:41:51 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	save_heredoc(t_cmd *cmd, t_token **tok)
{
	int	i;

	if (!*tok || (*tok)->type != T_WORD)
		return (ft_putstr_fd("minishell: syntax error near redirection\n", 2),
			-1);
	i = cmd->heredoc_count;
	cmd->heredoc_delims[i] = ft_strdup((*tok)->value);
	if (!cmd->heredoc_delims[i])
		return (-1);
	cmd->heredoc_expands[i] = ((*tok)->quoted == 0);
	cmd->heredoc_count++;
	*tok = (*tok)->next;
	return (0);
}

static int	set_infile(t_cmd *cmd, char *value)
{
	int	fd;

	fd = open(value, O_RDONLY);
	if (fd == -1)
	{
		perror(value);
		free(value);
		g_exit_status = 1;
		cmd->invalid_redir = 1;
		return (0);
	}
	close(fd);
	free(cmd->infile);
	cmd->infile = value;
	return (0);
}

static int	set_outfile(t_cmd *cmd, char *value, int append)
{
	int	fd;

	if (append)
		fd = open(value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(value);
		free(value);
		g_exit_status = 1;
		cmd->invalid_redir = 1;
		return (0);
	}
	close(fd);
	if (cmd->outfile)
		free(cmd->outfile);
	if (!cmd->invalid_redir)
	{
		cmd->outfile = value;
		cmd->append = append;
	}
	else
		free(value);
	return (0);
}

static int	ambiguous_redirect(t_cmd *cmd, char *value)
{
	if (value)
		free(value);
	ft_putstr_fd("minishell: ambiguous redirect\n", 2);
	g_exit_status = 1;
	cmd->invalid_redir = 1;
	return (0);
}

int	handle_redirection(t_cmd *cmd, t_token **tok, t_env *env)
{
	int		type;
	int		append;
	int		skip;
	char	*value;

	type = (*tok)->type;
	append = (type == T_APPEND);
	skip = 0;
	*tok = (*tok)->next;
	if (type == T_HEREDOC)
		return (save_heredoc(cmd, tok));
	if (!*tok || (*tok)->type != T_WORD)
		return (ft_putstr_fd("minishell: syntax error near redirection\n", 2),
			-1);
	value = join_expanded_arg(tok, env, &skip);
	if (!value && !skip)
		return (-1);
	if (!value || skip)
		return (ambiguous_redirect(cmd, value));
	if (cmd->invalid_redir)
		return (free(value), 0);
	if (type == T_REDIR_IN)
		return (set_infile(cmd, value));
	return (set_outfile(cmd, value, append));
}
