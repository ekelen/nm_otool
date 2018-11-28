/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 11:58:15 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/08 20:58:07 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char *cptr1;
	unsigned char *cptr2;

	cptr1 = (unsigned char *)s1;
	cptr2 = (unsigned char *)s2;
	while ((int)n > 0)
	{
		n--;
		if (*cptr1 == *cptr2)
		{
			cptr1++;
			cptr2++;
		}
		else
			return (*cptr1 - *cptr2);
	}
	return (0);
}
