/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 17:57:03 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/20 18:24:09 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_env_var(char *str, int *i, t_env *env, char **res)
{
	int		start;
	char	*key;
	char	*val;
	char	*to_append;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	key = ft_substr(str, start, *i - start);
	if (!key)
	{
		free(*res);
		*res = NULL;
		return (-1);
	}
	val = get_env_value(env, key);
	free(key);
	if (val)
		to_append = ft_strdup(val);
	else
		to_append = ft_strdup("");
	return (join_and_set(res, to_append));
}

static int	append_dollar(char *str, int *i, t_env *env, char **res)
{
	(*i)++;
	if (!str[*i] || (str[*i] != '?' && !ft_isalnum(str[*i]) && str[*i] != '_'))
		return (append_literal_char(res, '$'));
	if (str[*i] == '?')
		return (append_exit_status(res, i));
	return (append_env_var(str, i, env, res));
}

char	*expand_str(char *str, t_env *env)
{
	int		i;
	char	*res;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (append_dollar(str, &i, env, &res) == -1)
				return (NULL);
			continue ;
		}
		if (append_literal_char(&res, str[i]) == -1)
			return (NULL);
		i++;
	}
	return (res);
}

static char	*tilde(const char *s, t_env *env)
{
	const char	*home;
	char		*res;

	if (!s || s[0] != '~')
	{
		if (s)
			return (ft_strdup(s));
		return (ft_strdup(""));
	}
	if (s[1] && s[1] != '/')
		return (ft_strdup(s));
	home = get_env_value(env, "HOME");
	if (!home)
		return (ft_strdup(s));
	res = ft_strjoin2(home, s + 1);
	return (res);
}

int	expand_tokens(t_token *tok, t_env *env)
{
	char	*pre;
	char	*newv;

	while (tok)
	{
		if (tok->expand == 1 && tok->value)
		{
			if (tok->quoted == 0)
				pre = tilde(tok->value, env);
			else
				pre = ft_strdup(tok->value);
			if (!pre)
				return (-1);
			newv = expand_str(pre, env);
			free(pre);
			if (!newv)
				return (-1);
			free(tok->value);
			tok->value = newv;
		}
		tok = tok->next;
	}
	return (0);
}
