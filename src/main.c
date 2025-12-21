/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:26:34 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/21 13:21:48 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit_status = 0;

t_minishell	*minishell_init(char **env)
{
	t_minishell	*shell;

	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->input = NULL;
	shell->saved_stdin = NULL;
	shell->saved_stdout = NULL;
	shell->tokens = NULL;
	shell->in_pipeline = 0;
	shell->env = copy_env(env);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->fd_in = 0;
	shell->fd_out = 1;
	shell->fd_heredoc = 0;
	shell->exit_status = 0;
	return (shell);
}

static char	*read_input(void)
{
	char	*input;

	setup_sigreadline_handlers();
	input = readline("minishell> ");
	return (input);
}

static void	run_shell(t_minishell *shell)
{
	char	*input;

	while (1)
	{
		input = read_input();
		if (!input)
		{
			free(shell->input);
			break ;
		}
		shell->input = input;
		process_input(shell, &input);
		shell->input = input;
		free(shell->input);
		shell->input = NULL;
	}
	printf("exit\n");
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*shell;

	(void)argv;
	if (argc != 1)
		return (1);
	close_stray_fds();
	shell = minishell_init(env);
	if (!shell)
		return (1);
	run_shell(shell);
	free_tokens(&shell->tokens);
	free_env(shell->env);
	free(shell);
	rl_clear_history();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	return (0);
}
