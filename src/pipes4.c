/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:45:31 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 17:02:41 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prepare_pipeline(t_exec_ctx_2 *ctx)
{
	ctx->n = count_commands(ctx->cmd_list);
	ctx->fds = create_pipes(ctx->n);
	if (ctx->n > 1 && !ctx->fds)
		return (-1);
	if (process_all_heredocs(ctx->cmd_list, ctx->ms) == -1)
		return (-1);
	ctx->pids = malloc(sizeof(pid_t) * ctx->n);
	if (!ctx->pids)
		return (-1);
	return (0);
}

void	cleanup_pipeline_fds(t_exec_ctx_2 *ctx)
{
	if (ctx->fds)
	{
		close_fds(ctx->fds, ctx->n);
		free_pipes(ctx->fds, ctx->n);
	}
	close_all_heredoc_fds(ctx->cmd_list);
}

int	wait_pipeline(t_exec_ctx_2 *ctx)
{
	int	i;
	int	st;
	int	last_status;

	last_status = 0;
	i = 0;
	while (i < ctx->n)
	{
		waitpid(ctx->pids[i], &st, 0);
		if (i == ctx->n - 1)
			last_status = st;
		i++;
	}
	return (last_status);
}

void	init_exec_ctx_2(t_exec_ctx_2 *ctx, t_cmd *cmd_list, t_minishell *ms)
{
	ctx->cmd_list = cmd_list;
	ctx->ms = ms;
	ctx->fds = NULL;
	ctx->n = 0;
	ctx->pids = NULL;
}

void	update_exit_status(t_minishell *ms, int last_status)
{
	if (WIFEXITED(last_status))
		ms->exit_status = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
	{
		if (WTERMSIG(last_status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		ms->exit_status = 128 + WTERMSIG(last_status);
	}
	g_exit_status = ms->exit_status;
}
