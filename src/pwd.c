#include "minishell.h"

void pwd(t_cmd *cmd)
{
    char cwd[4096];
    (void)cmd;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd");
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

void    builtin_cd(t_cmd *cmd, t_minishell **shell)
{
    char        **av = cmd->argv;
    const char  *prev = NULL;
    char        *prev_dup = prev ? ft_strdup(prev) : NULL;
    char        *path;
    t_minishell *sh = *shell;

    prev = get_env_value(sh->env, "PWD");
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

