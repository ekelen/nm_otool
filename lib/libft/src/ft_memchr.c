/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 10:50:22 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/08 20:56:55 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	p;
	char			*p_s;

	p_s = (char *)s;
	p = (unsigned char)c;
	if (!n)
		return (NULL);
	if (sizeof(s) / n == sizeof(char *))
		return (((void *)ft_strchr(p_s, c)));
	else
	{
		i = 0;
		while (i < n)
		{
			if (((unsigned char *)s)[i] == p)
				return (((unsigned char *)s + i));
			i++;
		}
		return (NULL);
	}
}
