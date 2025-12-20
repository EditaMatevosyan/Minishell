/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:46:55 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 17:05:12 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_pipeline(t_exec_ctx_2 *ctx)
{
	int	last_status;

	if (prepare_pipeline(ctx) == -1)
	{
		cleanup_pipeline_fds(ctx);
		return (-1);
	}
	if (fork_and_execute(ctx) == -1)
	{
		free(ctx->pids);
		cleanup_pipeline_fds(ctx);
		return (-1);
	}
	cleanup_pipeline_fds(ctx);
	last_status = wait_pipeline(ctx);
	free(ctx->pids);
	update_exit_status(ctx->ms, last_status);
	return (0);
}

int	execute_pipeline(t_cmd *cmd_list, t_minishell *ms)
{
	t_exec_ctx_2	ctx;

	ms->in_pipeline = 1;
	setup_sigexecute_handlers();
	init_exec_ctx_2(&ctx, cmd_list, ms);
	if (run_pipeline(&ctx) == -1)
		return (-1);
	ms->in_pipeline = 0;
	setup_sigreadline_handlers();
	return (0);
}
