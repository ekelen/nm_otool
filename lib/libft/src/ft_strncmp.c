/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/09 16:27:14 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/09 16:27:24 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char src1_cpy;
	unsigned char src2_cpy;

	while (n-- > 0)
	{
		src1_cpy = (unsigned char)*s1++;
		src2_cpy = (unsigned char)*s2++;
		if (src1_cpy != src2_cpy)
			return (src1_cpy - src2_cpy);
		if (src1_cpy == '\0')
			return (0);
	}
	return (0);
}
