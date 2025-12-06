/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_ut1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:45:09 by edmatevo          #+#    #+#             */
/*   Updated: 2025/12/06 16:59:08 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_special(char c)
{
    if (c == '|' || c == '<' || c == '>')
        return 1;
    return 0;
}

int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char *ft_strndup(char *s, int n)
{
    char *dup;
    int i;

    dup = malloc(n+1);
    if(!dup)
        return NULL;
    i = 0;
    while(i < n && s[i])
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}

char *ft_strdup(const char *s)
{
    char *dup;
    int len = ft_strlen(s);
    int i = 0;

    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}
