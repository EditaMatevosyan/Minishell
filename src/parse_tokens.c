/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:12:26 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 18:30:11 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_token *tok)
{
	int	count;

	count = 0;
	while (tok && tok->type != T_PIPE)
	{
		if (tok->type == T_WORD)
			count++;
		tok = tok->next;
	}
	return (count);
}

t_cmd	*parse_tokens(t_token *tokens, t_env *env)
{
	t_cmd	*cmd_list;
	t_cmd	*last;
	t_cmd	*cmd;

	cmd_list = NULL;
	last = NULL;
	while (tokens)
	{
		cmd = parse_command(&tokens, env);
		if (!cmd)
		{
			free_cmd_list(&cmd_list);
			return (NULL);
		}
		if (!cmd_list)
			cmd_list = cmd;
		else
			last->next = cmd;
		last = cmd;
		if (tokens && tokens->type == T_PIPE)
			tokens = tokens->next;
	}
	return (cmd_list);
}
