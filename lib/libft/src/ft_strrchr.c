/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 14:49:03 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/09 16:32:25 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t i;
	size_t len;

	len = ft_strlen(s);
	i = len;
	while (i > 0 && s[i] != (char)c)
		--i;
	if (s[i] == (char)c)
		return ((char *)s + i);
	return (NULL);
}
