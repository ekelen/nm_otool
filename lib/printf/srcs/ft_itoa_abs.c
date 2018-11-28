/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_abs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 16:18:47 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 16:19:02 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char		*fill_itoa(char *itoad, uintmax_t nbr, int i)
{
	int flag;

	flag = 1;
	itoad[i] = '\0';
	i--;
	if (nbr == 0)
		itoad[i] = '0';
	while (i >= 0 && nbr != 0)
	{
		itoad[i--] = flag * (nbr % 10) + '0';
		nbr /= 10;
	}
	return (itoad);
}

static int		numlength(uintmax_t nbr)
{
	int		num_len;

	num_len = 0;
	if (nbr <= 0)
		num_len++;
	while (nbr != 0)
	{
		nbr /= 10;
		num_len++;
	}
	return (num_len);
}

char			*ft_itoa_abs(intmax_t n, t_arg *tg)
{
	char		*itoad;
	int			i;
	uintmax_t	nbr;

	tg->is_negative = n < 0 ? 1 : 0;
	nbr = tg->is_negative ? -n : n;
	i = numlength(nbr);
	itoad = (char *)malloc(sizeof(char) * (i + 1));
	if (!itoad)
		return ((void*)0);
	itoad = fill_itoa(itoad, nbr, i);
	return (itoad);
}
