/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 13:28:25 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 13:30:15 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	acc_digits(const char **ps, int sign, long long *acc_out)
{
	const char	*s;
	long long	acc;
	int			d;
	int			have;

	s = *ps;
	acc = 0;
	have = 0;
	while (*s >= '0' && *s <= '9')
	{
		d = *s - '0';
		if (sign == 1 && acc > (LLONG_MAX - d) / 10)
			return (0);
		if (sign == -1 && - acc < (LLONG_MIN + d) / 10)
			return (0);
		acc = acc * 10 + d;
		s++;
		have = 1;
	}
	if (!have)
		return (0);
	*ps = s;
	*acc_out = acc;
	return (1);
}

void	cleanup_and_exit(t_minishell *shell, int status)
{
	if (shell->tokens)
		free_tokens(&shell->tokens);
	if (shell->env)
		free_env(shell->env);
	if (shell->input)
		free(shell->input);
	if (shell->saved_stdin)
		close(*(shell->saved_stdin));
	if (shell->saved_stdout)
		close(*(shell->saved_stdout));
	if (shell->fd_in != -1 && shell->fd_in != 0)
		close(shell->fd_in);
	if (shell->fd_out != -1 && shell->fd_out != 1)
		close(shell->fd_out);
	if (shell->fd_heredoc != -1)
		close(shell->fd_heredoc);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	rl_clear_history();
	free(shell);
	exit(status);
}
