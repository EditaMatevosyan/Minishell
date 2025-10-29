/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_ut2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmatevo <edmatevo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 15:11:32 by edmatevo          #+#    #+#             */
/*   Updated: 2025/10/27 16:23:13 by edmatevo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *s)
{
    size_t	len;

    len = 0;
    while (s[len])
        len++;
    return (len);
}

int ft_strcpy(char *dst, const char *src)
{
    int i;

    i = 0;
    while (src[i])
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (i);
}

int ft_strcat(char *dst, const char *src)
{
    int i;
    int j;

    i = 0;
    while (dst[i])
        i++;
    j = 0;
    while (src[j])
    {
        dst[i + j] = src[j];
        j++;
    }
    dst[i + j] = '\0';
    return (i + j);
}

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int ft_strncmp(char *s1, char *s2, size_t n)
{
    size_t i;

    i = 0;
    while (i < n && s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    if (i < n)
        return ((unsigned char)s1[i] - (unsigned char)s2[i]);
    return (0);
}


