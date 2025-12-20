/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:40:38 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 18:40:38 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_heredocs(t_cmd *cmd)
{
	int	j;

	if (!cmd->heredoc_delims)
		return ;
	j = 0;
	while (j < cmd->heredoc_count)
	{
		if (cmd->heredoc_delims[j])
			free(cmd->heredoc_delims[j]);
		j++;
	}
	free(cmd->heredoc_delims);
	cmd->heredoc_delims = NULL;
	if (cmd->heredoc_fds)
		free(cmd->heredoc_fds);
	if (cmd->heredoc_expands)
		free(cmd->heredoc_expands);
	cmd->heredoc_fds = NULL;
	cmd->heredoc_expands = NULL;
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv);
	}
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	free_heredocs(cmd);
	free(cmd);
}

void	free_cmd_list(t_cmd **list)
{
	t_cmd	*tmp;
	t_cmd	*next;

	if (!list || !*list)
		return ;
	tmp = *list;
	while (tmp)
	{
		next = tmp->next;
		free_cmd(tmp);
		tmp = next;
	}
	*list = NULL;
}
