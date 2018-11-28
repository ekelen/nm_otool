/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/04 17:03:26 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/10 13:22:59 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t nwords;
	size_t is_start;

	nwords = 0;
	is_start = 0;
	while (*s)
	{
		if (*s != c && is_start == 0)
		{
			is_start = 1;
			nwords++;
		}
		if (*s == c && is_start == 1)
		{
			is_start = 0;
		}
		s++;
	}
	return (nwords);
}

static size_t	count_len(char const *s, size_t *index, char c)
{
	size_t len;

	len = 0;
	while (s[*index] == c)
	{
		(*index)++;
	}
	while (s[*index] != c && s[*index])
	{
		len++;
		(*index)++;
	}
	return (len);
}

char			**ft_strsplit(char const *s, char c)
{
	size_t	i;
	size_t	j;
	char	**str;
	size_t	nwords;
	size_t	nchars;

	if (!s)
		return ((void *)0);
	nwords = count_words(s, c);
	if (!(str = (char **)malloc(sizeof(char *) * (nwords + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (i < nwords && j < ft_strlen(s))
	{
		nchars = count_len(s, &j, c);
		if (!(str[i] = (char *)malloc(sizeof(char) * (nchars + 1))))
			return (NULL);
		ft_strncpy(str[i], s + (j - nchars), nchars);
		str[i][nchars] = '\0';
		i++;
	}
	str[i] = NULL;
	return (str);
}
