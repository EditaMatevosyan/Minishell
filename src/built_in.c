/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:02:28 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 12:50:44 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (!ft_strcmp(cmd->argv[0], "echo"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "export"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "env"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "exit"))
		return (1);
	return (0);
}

static int	dispatch_builtin(t_cmd *cmd, t_env **env, t_minishell *shell)
{
	char	**argv;

	argv = cmd->argv;
	if (!ft_strcmp(argv[0], "echo"))
		return (echo(cmd));
	if (!ft_strcmp(argv[0], "pwd"))
		return (pwd(cmd));
	if (!ft_strcmp(argv[0], "env"))
		return (env_print(*env, argv));
	if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exit(cmd, shell), shell->exit_status);
	if (!ft_strcmp(argv[0], "cd"))
		return (builtin_cd(cmd, shell), shell->exit_status);
	if (!ft_strcmp(argv[0], "export"))
		return (builtin_export(cmd, shell), shell->exit_status);
	if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset(cmd, shell), shell->exit_status);
	return (shell->exit_status);
}

int	exec_builtin(t_cmd *cmd, t_env **env, t_minishell *shell)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	shell->exit_status = dispatch_builtin(cmd, env, shell);
	return (shell->exit_status);
}
