/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/01 14:23:56 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/08 20:33:23 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	*dst0;
	unsigned char	*src0;
	size_t			i;
	unsigned char	p;

	p = (unsigned char)c;
	i = 0;
	if (dst == src)
		return (dst);
	dst0 = (unsigned char *)dst;
	src0 = (unsigned char *)src;
	while (i < n)
	{
		dst0[i] = src0[i];
		if (src0[i] == p)
			return (dst0 + i + 1);
		i++;
	}
	return ((void *)0);
}
