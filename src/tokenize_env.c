/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 13:04:24 by edmatevo          #+#    #+#             */
/*   Updated: 2025/10/27 16:49:35 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_env_value(t_env *env, char *key)
{
    while (env)
    {
        if (ft_strcmp(env->var, key) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

t_env *env_new(char *var, char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->var = ft_strdup(var);
    node->value = ft_strdup(value);
    node->next = NULL;
    return (node);
}

void env_add_back(t_env **env, t_env *new)
{
    t_env *tmp;

    if (!*env)
    {
        *env = new;
        return ;
    }
    tmp = *env;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}

t_env *env_init(char **envp)
{
    t_env   *env = NULL;
    int     i = 0;
    int     pos;
    char    *var;
    char    *value;

    while (envp[i])
    {
        pos = 0;
        while (envp[i][pos] && envp[i][pos] != '=')
            pos++;
        var = ft_substr(envp[i], 0, pos);
        value = ft_strdup(envp[i] + pos + 1);
        env_add_back(&env, env_new(var, value));
        free(var);
        free(value);
        i++;
    }
    return (env);
}
