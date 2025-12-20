/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 15:57:18 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 16:01:19 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

int	try_open_outfile(t_cmd *cmd, t_minishell *shell)
{
	int	fd;

	if (!cmd->outfile)
		return (0);
	fd = open_outfile(cmd);
	if (fd < 0)
	{
		shell->exit_status = 1;
		g_exit_status = 1;
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_empty_argv(t_cmd *cmd, t_minishell *shell)
{
	if (cmd->argv && cmd->argv[0])
		return (0);
	if (try_open_infile(cmd, shell) == -1)
		return (-1);
	if (try_open_outfile(cmd, shell) == -1)
		return (-1);
	return (1);
}

void	close_heredoc_fds(t_cmd *cmd)
{
	int	k;

	k = 0;
	while (k < cmd->heredoc_count)
	{
		if (cmd->heredoc_fds[k] != -1)
			close(cmd->heredoc_fds[k]);
		k++;
	}
}

void	execution_after_forking(t_cmd *cmd, t_minishell *shell)
{
	pid_t	pid;
	char	**envp_array;

	setup_sigexecute_handlers();
	envp_array = prepare_env(shell);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		shell->exit_status = 1;
		return ;
	}
	if (pid == 0)
		child_process(cmd, shell, envp_array);
	else
	{
		close_heredoc_fds(cmd);
		parent_process(pid, envp_array);
		shell->exit_status = g_exit_status;
	}
}

void	execute_command(t_cmd *cmd, t_minishell *shell)
{
	int		empty_argv;

	if (!cmd)
		return ;
	if (handle_invalid_redirs(cmd, shell) == -1)
		return ;
	empty_argv = handle_empty_argv(cmd, shell);
	if (empty_argv != 0)
		return ;
	if (cmd->heredoc_count > 0 && handle_heredoc(cmd, shell) == -1)
		return ;
	if (is_builtin(cmd))
	{
		return (execute_builtin_helper(cmd, shell));
	}
	execution_after_forking(cmd, shell);
}
