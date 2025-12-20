/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 15:50:15 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 15:58:26 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

int	handle_heredoc(t_cmd *cmd, t_minishell *shell)
{
	if (process_heredoc(cmd, shell->env) == -1)
		return (-1);
	return (0);
}

int	setup_builtin_redirs(t_cmd *cmd, t_minishell *shell,
		int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	shell->saved_stdin = saved_stdin;
	shell->saved_stdout = saved_stdout;
	if (change_stdin(cmd) == -1 || change_stdout(cmd) == -1)
		return (-1);
	return (0);
}

void	apply_builtin_heredoc(t_cmd *cmd)
{
	int	last;
	int	k;

	last = cmd->heredoc_count - 1;
	if (cmd->heredoc_fds[last] != -1)
		dup2(cmd->heredoc_fds[last], STDIN_FILENO);
	k = 0;
	while (k < cmd->heredoc_count)
	{
		if (cmd->heredoc_fds[k] != -1)
			close(cmd->heredoc_fds[k]);
		k++;
	}
}

void	restore_std_fds(t_minishell *shell, int saved_stdin,
		int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	shell->saved_stdin = NULL;
	shell->saved_stdout = NULL;
}
