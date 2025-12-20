/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 13:04:24 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 15:30:28 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->var, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	set_env_value(t_env **env, const char *key, const char *val)
{
	t_env	*cur;
	char	*dup;

	if (!env || !key || !val)
		return (1);
	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->var, (char *)key) == 0)
		{
			dup = ft_strdup((char *)val);
			if (!dup)
				return (1);
			free(cur->value);
			cur->value = dup;
			return (0);
		}
		cur = cur->next;
	}
	cur = env_new((char *)key, (char *)val);
	if (!cur)
		return (1);
	env_add_back(env, cur);
	return (0);
}

static int	extend_value(t_env *cur, const char *suffix)
{
	char	*joined;

	if (cur->value)
		joined = ft_strjoin(cur->value, (char *)suffix);
	else
		joined = ft_strjoin("", (char *)suffix);
	if (!joined)
		return (1);
	free(cur->value);
	cur->value = joined;
	return (0);
}

int	append_env_value(t_env **env, const char *key, const char *suffix)
{
	t_env	*cur;

	if (!env || !key || !suffix)
		return (1);
	cur = *env;
	while (cur && ft_strcmp(cur->var, (char *)key))
		cur = cur->next;
	if (cur)
		return (extend_value(cur, suffix));
	cur = env_new((char *)key, (char *)suffix);
	if (!cur)
		return (1);
	env_add_back(env, cur);
	return (0);
}

t_env	*env_init(char **envp)
{
	t_env	*env;
	int		i;
	int		pos;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		pos = 0;
		while (envp[i][pos] && envp[i][pos] != '=')
			pos++;
		if (envp[i][pos] == '=')
			env_add_back(&env, env_new(ft_substr(envp[i], 0, pos), envp[i] + pos
					+ 1));
		else
			env_add_back(&env, env_new(envp[i], ""));
		i++;
	}
	return (env);
}
