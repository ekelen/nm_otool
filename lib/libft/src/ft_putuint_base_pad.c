/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putuint_base_pad.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:47:24 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/07 12:42:44 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putuint_base_pad(uintmax_t nbr, int base, size_t w, char c)
{
	size_t len;

	len = ft_numlen_base(nbr, base, FALSE);
	while (len < w)
	{
		ft_putchar(c);
		len++;
	}
	ft_putuint_base(nbr, base);
	return ;
}
