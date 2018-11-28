/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/01 09:06:54 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/08 20:31:16 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	size_t	i;
	void	*mem_area;

	i = 0;
	mem_area = (char *)malloc(sizeof(char) * size);
	if (!mem_area)
		return (NULL);
	while (i < size)
	{
		((char *)mem_area)[i] = 0;
		i++;
	}
	(*(char *)mem_area) = '\0';
	return (mem_area);
}
