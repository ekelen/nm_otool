/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_countchar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/22 21:51:45 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/22 21:55:14 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_countchar(char *s, char c)
{
	int char_count;

	char_count = 0;
	while (*s)
	{
		if (*s == c)
			char_count++;
		s++;
	}
	return (char_count);
}
