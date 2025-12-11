/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:12:17 by romargar          #+#    #+#             */
/*   Updated: 2025/12/11 18:22:08 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int env_print(t_env *env, char **envp)
{
	t_env	*current;

	ft_putstr_fd("[BUILTIN ENV CALLED]\n", 2);
	if (envp[1])
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		return 127;
	}
	current = env;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->var, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		current = current->next;
	}
	return 0;
}