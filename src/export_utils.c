/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:29:38 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 14:29:39 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putnstr_fd(char *s, int n, int fd)
{
	write(fd, s, n);
}

void	export_ident_error(const char *arg, t_minishell *sh)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	sh->exit_status = 1;
}

void	env_add_if_missing(t_env **env, char *key)
{
	if (!get_env_value(*env, key))
		env_add_back(env, env_new(key, NULL));
}
