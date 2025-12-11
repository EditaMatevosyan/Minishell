/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:12:58 by romargar          #+#    #+#             */
/*   Updated: 2025/12/11 18:28:31 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int pwd(t_cmd *cmd)
{
    char cwd[4096];
    (void)cmd;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
        return (0);
    }
    else
    {
        perror("getcwd");
        return (1);
    }
}

static int set_oldpwd_after_success(t_minishell *sh, const char *prev_pwd)
{
    if (prev_pwd && set_env_value(&sh->env, "OLDPWD", prev_pwd))
    {
        sh->exit_status = 1;
        return (1);
    }
    return (0);
}

void    builtin_cd(t_cmd *cmd, t_minishell *sh)
{
    char        **av = cmd->argv;
    const char  *prev = get_env_value(sh->env, "PWD");
    char        *prev_dup = prev ? ft_strdup(prev) : NULL;
    char        *path;

    if (cd_too_many_args(av, sh))
        return ;
    if (av[1] && ft_strcmp(av[1], "-") == 0)
    {
        const char *old = get_env_value(sh->env, "OLDPWD");
        if (!old)
            return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2),
                    (void)(sh->exit_status = 1));
        if (chdir(old) == -1)
            return (perror("minishell: cd"), (void)(sh->exit_status = 1));
        ft_putstr_fd((char *)old, 1);
        ft_putstr_fd("\n", 1);
        if (set_oldpwd_after_success(sh, prev_dup) || cd_update_pwd(sh))
            return (free(prev_dup));
        free(prev_dup);
        sh->exit_status = 0;
        return ;
    }
    path = cd_resolve_path(av, sh);
    if (!path)
        return (free(prev_dup));
    if (chdir(path) == -1)
        return (perror("minishell: cd"), free(path), free(prev_dup),
                (void)(sh->exit_status = 1));
    free(path);
    if (set_oldpwd_after_success(sh, prev_dup) || cd_update_pwd(sh))
        return (free(prev_dup));
    free(prev_dup);
    sh->exit_status = 0;
}
