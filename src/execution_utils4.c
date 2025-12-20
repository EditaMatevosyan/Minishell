/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:59:10 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 15:01:13 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	setup_child_io(t_cmd *cmd)
{
	int	last;
	int	k;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (change_stdin(cmd) == -1 || change_stdout(cmd) == -1)
		exit(1);
	if (cmd->heredoc_count > 0 && cmd->heredoc_fds)
	{
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
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	status = 0;
	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		perror("waitpid");
		status = 1 << 8;
		break ;
	}
	return (status);
}

static void	set_exit_from_status(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		if (sig == SIGQUIT)
			printf("Quit (core dumped)\n");
		g_exit_status = 128 + sig;
	}
	else if (WIFEXITED(status))
	{
		g_exit_status = WEXITSTATUS(status);
	}
}

void	parent_process(pid_t pid, char **envp_array)
{
	int	status;

	status = wait_for_child(pid);
	free_env_array(envp_array);
	set_exit_from_status(status);
	setup_sigreadline_handlers();
}
