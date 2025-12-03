/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:02:28 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/03 11:48:15 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
        pwd(cmd);
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
    return (shell->exit_status);
}
