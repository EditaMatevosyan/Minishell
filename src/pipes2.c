/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:45:17 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 16:50:54 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pipe_fds(int i, int n, int **fds)
{
	if (!fds)
		return ;
	if (i > 0)
		dup2(fds[i - 1][0], STDIN_FILENO);
	if (i < n - 1)
		dup2(fds[i][1], STDOUT_FILENO);
	close_fds(fds, n);
}

void	setup_heredoc_fds(t_cmd *cmds)
{
	int	last_heredoc;
	int	k;

	if (cmds->heredoc_count <= 0)
		return ;
	last_heredoc = cmds->heredoc_count - 1;
	if (cmds->heredoc_fds && cmds->heredoc_fds[last_heredoc] != -1)
	{
		dup2(cmds->heredoc_fds[last_heredoc], STDIN_FILENO);
		k = 0;
		while (k < cmds->heredoc_count)
		{
			if (cmds->heredoc_fds[k] != -1)
				close(cmds->heredoc_fds[k]);
			k++;
		}
	}
}

void	finalize_stdio_or_exit(t_cmd *cmds, int **fds, int n)
{
	if (change_stdin(cmds) == -1 || change_stdout(cmds) == -1)
	{
		if (fds)
			free_pipes(fds, n);
		exit(1);
	}
}

void	setup_fds(t_cmd *cmds, int i, int n, int **fds)
{
	setup_child_signals();
	setup_pipe_fds(i, n, fds);
	setup_heredoc_fds(cmds);
	finalize_stdio_or_exit(cmds, fds, n);
}

int	process_all_heredocs(t_cmd *cmd_list, t_minishell *ms)
{
	t_cmd	*cur;

	cur = cmd_list;
	while (cur)
	{
		if (cur->heredoc_count > 0)
		{
			if (process_heredoc(cur, ms->env) == -1)
			{
				return (-1);
			}
		}
		cur = cur->next;
	}
	return (0);
}
