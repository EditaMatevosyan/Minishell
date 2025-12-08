#include "minishell.h"

int ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int is_valid_identifier(char *str)
{
    int i;

    i = 1;
    if (!str || !str[0])
        return (0);
    if (!(ft_isalpha(str[0]) || str[0] == '_'))
        return (0);
    while (str[i])
    {
        if (!(ft_isalnum(str[i]) || str[i] == '_'))
            return (0);
        i++;
    }
    return (1);
}

int env_remove(t_env **env, const char *name)
{
    t_env *current = *env;
    t_env *previous = NULL;

    while (current)
    {
        if (current->var && strcmp(current->var, name) == 0)
        {
            if (previous)
                previous->next = current->next;
            else
                *env = current->next;

            free(current->var);
            free(current->value);
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}


void builtin_unset(t_cmd *cmd, t_minishell **sh)
{
    char **av;
    int i;
    t_minishell *shell;

    av = cmd->argv;
    shell = *sh;
    shell->exit_status = 0;
    if (!av)
        return;

    // //debug
    // while(shell->env)
    // {
    //     printf("shell_env: %s\n", shell->env->value);
    //     shell->env = shell->env->next;
    // }
    i = 1;
    while (av[i])
    {
        // //debug
        // printf("av[i]: %s\n", av[i]);
        if (is_valid_identifier(av[i]))
        {
            env_remove(&shell->env, av[i]);
            //debug
            t_env *tmp = shell->env;
            while (tmp)
            {
                printf("env var: %s\n", tmp->var);
                tmp = tmp->next;
            }
        }
        else
        {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(av[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            shell->exit_status = 1;
        }
        i++;
    }
}
