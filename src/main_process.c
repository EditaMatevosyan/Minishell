/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:05:12 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 18:52:47 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	tokenize_and_validate(t_minishell *ms, char **input)
{
	int	rc;

	rc = tokenize_input(ms, input);
	if (rc == -1)
		return (-1);
	if (syntax_check(ms->tokens))
	{
		ms->exit_status = 2;
		g_exit_status = 2;
		return (-1);
	}
	if (expand_tokens(ms->tokens, ms->env) == -1)
		return (-1);
	return (0);
}

static void	execute_tokens(t_minishell *ms)
{
	t_cmd	*cmds;

	cmds = parse_tokens(ms->tokens, ms->env);
	if (!cmds)
	{
		ms->exit_status = g_exit_status;
		return ;
	}
	if (count_commands(cmds) == 1)
		execute_command(cmds, ms);
	else
		execute_pipeline(cmds, ms);
	free_cmd_list(&cmds);
}

void	process_input(t_minishell *ms, char **input)
{
	if (!input || !*input || **input == '\0')
		return ;
	add_history(*input);
	if (tokenize_and_validate(ms, input) == -1)
	{
		free_tokens(&ms->tokens);
		return ;
	}
	execute_tokens(ms);
	free_tokens(&ms->tokens);
}
