/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 13:10:44 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 13:10:45 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_update_env_after_chdir(t_minishell *sh, char *prev_dup)
{
	if (prev_dup && set_env_value(&sh->env, "OLDPWD", prev_dup))
	{
		free(prev_dup);
		sh->exit_status = 1;
		return (1);
	}
	if (cd_update_pwd(sh))
	{
		free(prev_dup);
		return (1);
	}
	free(prev_dup);
	sh->exit_status = 0;
	return (0);
}

int	cd_update_pwd(t_minishell *sh)
{
	char	buf[PATH_MAX];
	char	*cwd;

	cwd = getcwd(buf, sizeof(buf));
	if (!cwd)
	{
		perror("minishell: cd: getcwd");
		sh->exit_status = 1;
		return (1);
	}
	if (set_env_value(&sh->env, "PWD", cwd))
	{
		sh->exit_status = 1;
		return (1);
	}
	return (0);
}

static void	cd_switch_to_oldpwd(t_minishell *sh, const char *old,
		char *prev_dup)
{
	if (!old)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		free(prev_dup);
		sh->exit_status = 1;
		return ;
	}
	if (chdir(old) == -1)
	{
		perror("minishell: cd");
		free(prev_dup);
		sh->exit_status = 1;
		return ;
	}
	ft_putstr_fd((char *)old, 1);
	ft_putstr_fd("\n", 1);
	cd_update_env_after_chdir(sh, prev_dup);
}

static void	cd_switch_to_path(char **av, t_minishell *sh, char *prev_dup)
{
	char	*path;

	path = cd_resolve_path(av, sh);
	if (!path)
		return (free(prev_dup));
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		free(path);
		free(prev_dup);
		sh->exit_status = 1;
		return ;
	}
	free(path);
	cd_update_env_after_chdir(sh, prev_dup);
}

void	builtin_cd(t_cmd *cmd, t_minishell *sh)
{
	char		**av;
	char		*prev_dup;
	const char	*prev;
	const char	*old;

	av = cmd->argv;
	prev = get_env_value(sh->env, "PWD");
	old = get_env_value(sh->env, "OLDPWD");
	if (prev)
		prev_dup = ft_strdup(prev);
	else
		prev_dup = NULL;
	if (cd_too_many_args(av, sh))
		return (free(prev_dup));
	if (av[1] && ft_strcmp(av[1], "-") == 0)
		return (cd_switch_to_oldpwd(sh, old, prev_dup));
	cd_switch_to_path(av, sh, prev_dup);
}
