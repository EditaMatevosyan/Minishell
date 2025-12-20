/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:13:11 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 15:30:04 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_minishell *shell)
{
	t_env *tmp = shell->env;
	while (tmp)
	{
    	if (ft_strcmp(tmp->var, "PATH") == 0)
    	{
        	printf("PATH = %s\n", tmp->value);
        	break;
    	}
    	tmp = tmp->next;
	}
}
