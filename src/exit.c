/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 13:15:02 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 13:30:24 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_print_exit(t_minishell *shell)
{
	if (!shell->in_pipeline && isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
}

static void	ms_exit_numerr(t_minishell *shell, char *arg)
{
	ms_print_exit(shell);
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	free(arg);
	if (shell->in_pipeline)
	{
		shell->exit_status = 2;
		return ;
	}
	cleanup_and_exit(shell, 2);
}

static void	exit_no_arg(t_cmd *cmd, t_minishell *shell)
{
	ms_print_exit(shell);
	if (shell->in_pipeline)
		return ;
	free_cmd_list(&cmd);
	cleanup_and_exit(shell, (unsigned char)shell->exit_status);
}

static int	exit_parse_arg(char *arg, t_cmd *cmd, t_minishell *shell,
		long long *val)
{
	char	*arg_copy;

	if (ft_atoll(arg, val))
		return (1);
	arg_copy = ft_strdup(arg);
	if (!shell->in_pipeline)
		free_cmd_list(&cmd);
	if (!arg_copy)
	{
		if (shell->in_pipeline)
		{
			shell->exit_status = 255;
			return (0);
		}
		cleanup_and_exit(shell, 255);
	}
	ms_exit_numerr(shell, arg_copy);
	return (0);
}

void	builtin_exit(t_cmd *cmd, t_minishell *shell)
{
	char		**av;
	long long	val;

	av = cmd->argv;
	if (!av || !av[0])
		return ;
	if (!av[1])
		return (exit_no_arg(cmd, shell));
	if (!exit_parse_arg(av[1], cmd, shell, &val))
		return ;
	if (av[2])
	{
		ms_print_exit(shell);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return ;
	}
	ms_print_exit(shell);
	if (shell->in_pipeline)
	{
		shell->exit_status = (unsigned char)val;
		return ;
	}
	free_cmd_list(&cmd);
	cleanup_and_exit(shell, (unsigned char)val);
}
