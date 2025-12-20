/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:26:31 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 14:32:15 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

static char	*find_exec_in_dirs(char **directories, char *command)
{
	int		i;
	char	*full_path;

	i = 0;
	while (directories[i])
	{
		full_path = join_path(directories[i], command);
		if (full_path && access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_full_path(t_cmd *cmd, t_env *env)
{
	char	*path_value;
	char	**directories;
	char	*full_path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (NULL);
	if (strchr(cmd->argv[0], '/'))
		return (strdup(cmd->argv[0]));
	path_value = get_env_value(env, "PATH");
	if (!path_value)
		return (NULL);
	directories = ft_split(path_value, ':');
	if (!directories)
		return (NULL);
	full_path = find_exec_in_dirs(directories, cmd->argv[0]);
	free_split(directories);
	return (full_path);
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

static void	fill_env_array(t_env *env, char **arr)
{
	int		i;
	char	*tmp2;

	i = 0;
	while (env)
	{
		arr[i] = ft_strjoin(env->var, "=");
		tmp2 = arr[i];
		if (env->value)
			arr[i] = ft_strjoin(tmp2, env->value);
		else
			arr[i] = ft_strjoin(tmp2, "");
		free(tmp2);
		env = env->next;
		i++;
	}
}

char	**env_list_to_array(t_env *env)
{
	int		len;
	char	**arr;

	len = env_list_len(env);
	arr = malloc(sizeof(char *) * (len + 1));
	if (!arr)
		return (NULL);
	fill_env_array(env, arr);
	arr[len] = NULL;
	return (arr);
}
