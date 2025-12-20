/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:12:08 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 12:53:19 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_too_many_args(char **av, t_minishell *sh)
{
	if (av[1] && av[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		sh->exit_status = 1;
		return (1);
	}
	return (0);
}

char	*cd_dup_home(t_minishell *sh)
{
	const char	*home = get_env_value(sh->env, "HOME");
	char		*out;

	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		sh->exit_status = 1;
		return (NULL);
	}
	out = ft_strdup(home);
	if (!out)
		sh->exit_status = 1;
	return (out);
}

char	*cd_expand_tilde(const char *arg, t_minishell *sh)
{
	char	*home;
	char	*rest;
	char	*out;

	if (!arg || arg[0] != '~')
	{
		if (arg)
			return (ft_strdup(arg));
		return (ft_strdup(""));
	}
	home = cd_dup_home(sh);
	if (!home)
		return (NULL);
	rest = ft_strdup(arg + 1);
	if (!rest)
	{
		free(home);
		sh->exit_status = 1;
		return (NULL);
	}
	out = str_join_free(home, rest);
	if (!out)
		sh->exit_status = 1;
	return (out);
}

char	*cd_resolve_path(char **av, t_minishell *sh)
{
	if (!av[1])
		return (cd_dup_home(sh));
	if (av[1][0] == '~')
		return (cd_expand_tilde(av[1], sh));
	return (ft_strdup(av[1]));
}

int	cd_set_oldpwd(t_minishell *sh)
{
	const char	*prev = get_env_value(sh->env, "PWD");

	if (prev && set_env_value(&sh->env, "OLDPWD", prev))
	{
		sh->exit_status = 1;
		return (1);
	}
	return (0);
}
