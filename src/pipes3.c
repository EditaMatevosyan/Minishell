/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:45:25 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 17:01:38 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_child(t_exec_ctx_2 *ctx, int code)
{
	if (ctx->fds)
		free_pipes(ctx->fds, ctx->n);
	free(ctx->pids);
	cleanup(ctx->cmd_list, ctx->ms);
	exit(code);
}

void	exec_external_or_exit(t_cmd *cur, t_exec_ctx_2 *ctx)
{
	char	**envp;
	char	*path;

	envp = env_list_to_array(ctx->ms->env);
	path = get_full_path(cur, ctx->ms->env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cur->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_env_array(envp);
		exit_child(ctx, 127);
	}
	execve(path, cur->argv, envp);
	perror("execve");
	free_env_array(envp);
	free(path);
	exit_child(ctx, 126);
}

void	child_process_2(t_cmd *cur, int i, t_exec_ctx_2 *ctx)
{
	int	st;

	if (cur->invalid_redir)
		exit_child(ctx, 1);
	setup_fds(cur, i, ctx->n, ctx->fds);
	if (is_builtin(cur))
	{
		st = exec_builtin(cur, &ctx->ms->env, ctx->ms);
		exit_child(ctx, (unsigned char)st);
	}
	exec_external_or_exit(cur, ctx);
}

int	fork_and_execute(t_exec_ctx_2 *ctx)
{
	t_cmd	*cur;
	int		i;

	cur = ctx->cmd_list;
	i = 0;
	while (cur)
	{
		ctx->pids[i] = fork();
		if (ctx->pids[i] < 0)
			return (-1);
		if (ctx->pids[i] == 0)
			child_process_2(cur, i, ctx);
		cur = cur->next;
		i++;
	}
	return (0);
}

void	close_all_heredoc_fds(t_cmd *cmd_list)
{
	t_cmd	*cur;
	int		j;

	cur = cmd_list;
	while (cur)
	{
		if (cur->heredoc_fds)
		{
			j = 0;
			while (j < cur->heredoc_count)
			{
				if (cur->heredoc_fds[j] != -1)
				{
					close(cur->heredoc_fds[j]);
					cur->heredoc_fds[j] = -1;
				}
				j++;
			}
		}
		cur = cur->next;
	}
}
