/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strncat.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 14:10:15 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/09 16:26:53 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	size_t i;
	size_t size;

	i = 0;
	size = 0;
	while (s1[size])
		size++;
	while (s2[i] && i < n)
	{
		s1[size] = s2[i];
		i++;
		size++;
	}
	s1[size] = '\0';
	return (s1);
}
