/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:12:52 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 17:29:18 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_prev_heredocs(t_cmd *cmd, int i)
{
	while (--i >= 0)
		if (cmd->heredoc_fds[i] != -1)
			close(cmd->heredoc_fds[i]);
}

static void	write_heredoc_line(t_heredoc_ctx *ctx, char *line)
{
	char	*expanded;

	if (ctx->cmd->heredoc_expands[ctx->idx] == 1)
		expanded = expand_str(line, ctx->env);
	else
		expanded = ft_strdup(line);
	if (!expanded)
		expanded = ft_strdup("");
	write(ctx->fd_out, expanded, ft_strlen(expanded));
	write(ctx->fd_out, "\n", 1);
	free(expanded);
}

static void	read_heredoc_lines(t_heredoc_ctx *ctx)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, ctx->cmd->heredoc_delims[ctx->idx]) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(ctx, line);
		free(line);
	}
}

static int	open_heredoc_pipe(t_cmd *cmd, int i, int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		close_prev_heredocs(cmd, i);
		return (-1);
	}
	return (0);
}

int	process_heredoc(t_cmd *cmd, t_env *env)
{
	int				fd[2];
	int				i;
	t_heredoc_ctx	ctx;

	i = 0;
	while (i < cmd->heredoc_count)
	{
		if (open_heredoc_pipe(cmd, i, fd) == -1)
			return (-1);
		ctx.cmd = cmd;
		ctx.env = env;
		ctx.idx = i;
		ctx.fd_out = fd[1];
		read_heredoc_lines(&ctx);
		close(fd[1]);
		if (cmd->heredoc_fds[i] != -1)
			close(cmd->heredoc_fds[i]);
		cmd->heredoc_fds[i] = fd[0];
		i++;
	}
	return (0);
}
