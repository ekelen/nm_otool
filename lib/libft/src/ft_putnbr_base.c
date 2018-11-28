/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:03:05 by ekelen            #+#    #+#             */
/*   Updated: 2018/10/19 14:07:30 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_base(intmax_t nbr, int base)
{
	char const *digits = "0123456789abcdef";

	if (nbr / base > 1)
		ft_putnbr_base(nbr / base, base);
	write(1, digits + nbr % base, 1);
}