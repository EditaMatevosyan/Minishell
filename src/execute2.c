/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 15:58:47 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 15:59:19 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	handle_builtin_redir_failure(t_minishell *shell, int saved_stdin,
		int saved_stdout)
{
	restore_std_fds(shell, saved_stdin, saved_stdout);
	shell->exit_status = g_exit_status;
}

void	execute_builtin_helper(t_cmd *cmd, t_minishell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (setup_builtin_redirs(cmd, shell, &saved_stdin, &saved_stdout) == -1)
	{
		handle_builtin_redir_failure(shell, saved_stdin, saved_stdout);
		return ;
	}
	if (cmd->heredoc_count > 0 && cmd->heredoc_fds)
		apply_builtin_heredoc(cmd);
	status = exec_builtin(cmd, &shell->env, shell);
	shell->exit_status = (unsigned char)status;
	g_exit_status = shell->exit_status;
	restore_std_fds(shell, saved_stdin, saved_stdout);
	setup_sigreadline_handlers();
}

char	**prepare_env(t_minishell *shell)
{
	char	**envp_array;

	envp_array = env_list_to_array(shell->env);
	if (!envp_array)
	{
		perror("malloc");
		shell->exit_status = 1;
		exit(1);
	}
	return (envp_array);
}

int	handle_invalid_redirs(t_cmd *cmd, t_minishell *shell)
{
	if (!cmd->invalid_redir)
		return (0);
	shell->exit_status = 1;
	g_exit_status = 1;
	return (-1);
}

int	try_open_infile(t_cmd *cmd, t_minishell *shell)
{
	int	fd;

	if (!cmd->infile)
		return (0);
	fd = open_infile(cmd);
	if (fd < 0)
	{
		shell->exit_status = 1;
		g_exit_status = 1;
		return (-1);
	}
	close(fd);
	return (0);
}
