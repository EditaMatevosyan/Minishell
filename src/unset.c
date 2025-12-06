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

void env_remove(t_env **env, char *var)
{
    t_env *current;
    t_env *previous;

    current = *env;
    previous = NULL;
    while (current)
    {
        if (ft_strcmp(current->var, var) == 0)
        {
            if (previous)
                previous->next = current->next;
            else
                *env = current->next;
            free(current->var);
            free(current->value);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void builtin_unset(t_cmd *cmd, t_minishell *shell)
{
    char **av;
    int i;

    av = cmd->argv;
    shell->exit_status = 0;
    if (!av)
        return;
    i = 0;
    while (av[i])
    {
        if (is_valid_identifier(av[i]))
            env_remove(&shell->env, av[i]);
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
