/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romargar <romargar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:12:47 by romargar          #+#    #+#             */
/*   Updated: 2025/12/20 16:43:08 by romargar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*cur;

	count = 0;
	cur = cmd_list;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

int	**create_pipes(int n)
{
	int	**fds;
	int	i;

	if (n <= 1)
		return (NULL);
	fds = malloc(sizeof(int *) * (n - 1));
	if (!fds)
		return (NULL);
	i = 0;
	while (i < n - 1)
	{
		fds[i] = malloc(sizeof(int) * 2);
		if (!fds[i])
			return (NULL);
		if (pipe(fds[i]) == -1)
			return (NULL);
		i++;
	}
	return (fds);
}

void	free_pipes(int **fds, int n)
{
	int	i;

	if (!fds)
		return ;
	i = 0;
	while (i < n - 1)
	{
		if (fds[i])
			free(fds[i]);
		i++;
	}
	free(fds);
}

void	close_fds(int **fds, int n)
{
	int	i;

	if (!fds)
		return ;
	i = 0;
	while (i < n - 1)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
