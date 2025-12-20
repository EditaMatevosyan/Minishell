/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:29:53 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 14:29:54 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_str_array(char **arr, int count)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static int	env_list_len(t_env *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

static char	*env_entry_to_str(t_env *node)
{
	char	*with_eq;
	char	*out;

	if (node->value == NULL)
		return (ft_strdup(node->var));
	with_eq = ft_strjoin(node->var, "=");
	if (!with_eq)
		return (NULL);
	out = ft_strjoin(with_eq, node->value);
	free(with_eq);
	return (out);
}

static int	fill_env_array(t_env *env, char **arr)
{
	int	i;

	i = 0;
	while (env)
	{
		arr[i] = env_entry_to_str(env);
		if (!arr[i])
			return (free_str_array(arr, i), 0);
		env = env->next;
		i++;
	}
	arr[i] = NULL;
	return (1);
}

char	**env_list_to_array_for_export(t_env *env)
{
	int		len;
	char	**arr;

	len = env_list_len(env);
	arr = malloc(sizeof(char *) * (len + 1));
	if (!arr)
		return (NULL);
	if (!fill_env_array(env, arr))
		return (NULL);
	return (arr);
}
