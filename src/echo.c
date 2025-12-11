/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 15:56:18 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/11 18:21:57 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int n_flag(char *str)
{
    int i = 0;
    if (str[i] != '-')
        return 0;
    i = 1;
    while (str[i] == 'n')
        i++;
    return (str[i] == '\0');
}

int echo(t_cmd *cmd)
{
    int i = 1;
    int newline = 1;

    if (!cmd->argv)
        return 0;
    while (cmd->argv[i] && n_flag(cmd->argv[i]))
    {
        newline = 0;
        i++;
    }
    while (cmd->argv[i])
    {
        ft_putstr_fd(cmd->argv[i], 1);
        if (cmd->argv[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (newline)
        ft_putstr_fd("\n", 1);
    return 0;
}
