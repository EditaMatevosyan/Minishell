/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:02:28 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/07 13:50:17 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

int is_builtin(t_cmd *cmd)
{
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return 0;

    if (!ft_strcmp(cmd->argv[0], "echo"))
        return 1;
    if (!ft_strcmp(cmd->argv[0], "cd"))
        return 1;
    if (!ft_strcmp(cmd->argv[0], "pwd"))
        return 1;
    if (!ft_strcmp(cmd->argv[0], "export"))
        return 1;
    if (!ft_strcmp(cmd->argv[0], "unset"))
        return 1;
    if (!ft_strcmp(cmd->argv[0], "env"))
        return 1;
    if (!ft_strcmp(cmd->argv[0], "exit"))
        return 1;

    return 0; // Not a built-in
}

int exec_builtin(t_cmd *cmd, t_env **env, t_minishell *shell)
{
    // (void)env;
    char **argv;
    
    argv = cmd->argv;
    if(!argv || !argv[0])
        return (0);
    if (!ft_strcmp(argv[0], "echo"))
    {
        echo(cmd);
        return(shell->exit_status);
    }
    if(!ft_strcmp(argv[0], "pwd"))
    {
        pwd(cmd, shell);
        return(shell->exit_status);
    }
    if(!ft_strcmp(argv[0], "env"))
    {
        env_print(*env, argv);
        return(shell->exit_status);
    }
    if(!ft_strcmp(argv[0], "exit"))
    {
        builtin_exit(cmd, shell);
        return(shell->exit_status);
    }
    if(!ft_strcmp(argv[0], "cd"))
    {
        builtin_cd(cmd, shell);
        return(shell->exit_status);
    }
    if(!ft_strcmp(argv[0], "export"))
    {
        builtin_export(cmd, shell);
        return(shell->exit_status);
    }
    if(!ft_strcmp(argv[0], "unset"))
    {
        builtin_unset(cmd, shell);
        return(shell->exit_status);
    }
    return (shell->exit_status);
}
