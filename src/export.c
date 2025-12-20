/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:12:22 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 15:27:42 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_key(char *arg, char *eq, int *append)
{
	if (eq > arg && *(eq - 1) == '+')
	{
		*append = 1;
		return (ft_substr(arg, 0, eq - arg - 1));
	}
	return (ft_substr(arg, 0, eq - arg));
}

static int	process_assignment(char *arg, char *eq, t_minishell *sh)
{
	int		append;
	char	*key;

	append = 0;
	key = extract_key(arg, eq, &append);
	if (!key)
		return (sh->exit_status = 1, 0);
	if (!is_valid_identifier(key))
		export_ident_error(arg, sh);
	else if (append)
		append_env_value(&sh->env, key, eq + 1);
	else
		set_env_value(&sh->env, key, eq + 1);
	free(key);
	return (1);
}

static void	process_no_equal(char *arg, t_minishell *sh)
{
	if (!is_valid_identifier(arg))
		export_ident_error(arg, sh);
	else
		env_add_if_missing(&sh->env, arg);
}

void	builtin_export(t_cmd *cmd, t_minishell *sh)
{
	int		i;
	char	*arg;
	char	*eq;

	sh->exit_status = 0;
	if (!cmd->argv[1])
		return (export_print(sh->env), (void)(sh->exit_status = 0));
	i = 1;
	while (cmd->argv[i])
	{
		arg = cmd->argv[i];
		eq = ft_strchr(arg, '=');
		if (!eq)
			process_no_equal(arg, sh);
		else if (!process_assignment(arg, eq, sh))
			return ;
		i++;
	}
}
