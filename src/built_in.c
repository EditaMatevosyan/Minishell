/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:02:28 by edmatevo          #+#    #+#             */
/*   Updated: 2025/11/12 17:47:17 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_builtin(t_cmd *cmd, t_env **env)
{
    (void)env;
    char **argv = cmd->argv;

    if(!argv || !argv[0])
        return (0);
    
    if (!ft_strcmp(argv[0], "echo"))
    {
        echo(cmd);
        return (1);
    }
    return (0);
}
