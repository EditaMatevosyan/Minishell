/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_join.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:20:38 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 18:20:38 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	concat_parts(char **arg, char *part)
{
	char	*joined;

	if (!*arg)
	{
		*arg = part;
		return (0);
	}
	joined = malloc(ft_strlen(*arg) + ft_strlen(part) + 1);
	if (!joined)
	{
		free(*arg);
		free(part);
		return (-1);
	}
	ft_strcpy(joined, *arg);
	ft_strcat(joined, part);
	free(*arg);
	free(part);
	*arg = joined;
	return (0);
}

static int	append_part_token(t_token *t, t_env *env, char **arg, int *state)
{
	char	*part;

	if (t->quoted == 2)
		part = ft_strdup(t->value);
	else if (t->expand)
		part = expand_str(t->value, env);
	else
		part = ft_strdup(t->value);
	if (!part)
		return (-1);
	if (t->quoted != 0 || t->expand == 0)
		state[0] = 0;
	if (part[0] != '\0')
		state[1] = 1;
	if (concat_parts(arg, part) == -1)
		return (-1);
	return (0);
}

static int	join_loop(t_token **tok, t_env *env, char **arg, int *state)
{
	t_token	*t;

	t = *tok;
	while (t && t->type == T_WORD)
	{
		if (append_part_token(t, env, arg, state) == -1)
			return (-1);
		t = t->next;
		if (!t || t->type != T_WORD || t->glued == 0)
			break ;
	}
	*tok = t;
	return (0);
}

char	*join_expanded_arg(t_token **tok, t_env *env, int *skip_arg)
{
	char	*arg;
	int		state[2];

	arg = NULL;
	state[0] = 1;
	state[1] = 0;
	if (skip_arg)
		*skip_arg = 0;
	if (join_loop(tok, env, &arg, state) == -1)
		return (free(arg), NULL);
	if (!state[1] && state[0])
	{
		free(arg);
		if (skip_arg)
			*skip_arg = 1;
		return (NULL);
	}
	return (arg);
}
