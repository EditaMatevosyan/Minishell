/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 14:50:22 by edmatevo          #+#    #+#             */
/*   Updated: 2025/10/26 16:34:36 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *copy_env(char **envp)
{
    t_env *head = NULL;
    t_env *node;
    int i = 0;

    while (envp[i])
    {
        node = malloc(sizeof(t_env));
        if (!node)
            return (NULL);
        char *eq = ft_strchr(envp[i], '=');
        if (eq)
        {
            node->var = ft_substr(envp[i], 0, eq - envp[i]);
            node->value = ft_strdup(eq + 1);
        }
        else
        {
            node->var = ft_strdup(envp[i]);
            node->value = NULL;
        }
        node->next = NULL;
        env_add_back(&head, node);
        i++;
    }
    return (head);
}
void free_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        free(env->var);
        free(env->value);
        free(env);
        env = tmp;
    }
}
