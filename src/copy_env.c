/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 14:50:22 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/03 10:05:38 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env *env_node_new(char *s)
{
    t_env *node;
    char *eq;

    node = malloc(sizeof(*node));
    if (!node)
        return (NULL);
    eq = ft_strchr(s, '=');
    if(eq)
    {
        node->var = ft_substr(s, 0, eq - s);
        if (!node->var)
            return (free(node), NULL);
        node->value = ft_strdup(eq + 1);
        if (!node->value)
            return (free(node->var), free(node), NULL);
    }
    else
    {
        node->var = ft_strdup(s);
        if (!node->var)
            return (free(node), NULL);
        node->value = NULL;
    }
    node->next = NULL;
    return (node);
}

t_env *copy_env(char **envp)
{
    t_env *head;
    t_env *node;
    int i;

    head = NULL;
    i = 0;
    while (envp[i])
    {
        node = env_node_new(envp[i]);
        if (!node)
        {
            free_env(head);
            return (NULL);
        }
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
