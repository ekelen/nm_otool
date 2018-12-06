/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putuint_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:47:24 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/06 21:14:42 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// #include <stdio.h>

void	ft_putuint_base(uintmax_t nbr, int base)
{
	char const *digits = "0123456789abcdef";

	if (nbr / base >= 1)
		ft_putuint_base(nbr / base, base);
	write(1, digits + (nbr % base), 1);
}