/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:29:58 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 14:29:59 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_escaped(const char *s)
{
	while (*s)
	{
		if (*s == '\"' || *s == '\\' || *s == '$')
			ft_putchar_fd('\\', 1);
		ft_putchar_fd(*s++, 1);
	}
}

static void	print_export_line(char *entry)
{
	char	*eq;

	ft_putstr_fd("declare -x ", 1);
	eq = ft_strchr(entry, '=');
	if (!eq)
		ft_putstr_fd(entry, 1);
	else
	{
		ft_putnstr_fd(entry, (int)(eq - entry + 1), 1);
		ft_putstr_fd("\"", 1);
		print_escaped(eq + 1);
		ft_putstr_fd("\"", 1);
	}
	ft_putstr_fd("\n", 1);
}

void	export_print(t_env *env)
{
	char	**arr;
	int		i;

	arr = env_list_to_array_for_export(env);
	if (!arr)
		return ;
	sort_env_by_key(arr);
	i = 0;
	while (arr[i])
	{
		print_export_line(arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
}
