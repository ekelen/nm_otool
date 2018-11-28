/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_padding.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:25:59 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 21:26:10 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		write_padding(char padding, t_arg *tg, size_t num_spaces)
{
	size_t i;

	i = 0;
	while (i < num_spaces)
	{
		ft_putchar_fd(padding, 1);
		i++;
	}
	tg->len += i;
}

void		write_padding_static(char padding, size_t num_spaces)
{
	size_t i;

	i = 0;
	while (i < num_spaces)
	{
		ft_putchar_fd(padding, 1);
		i++;
	}
}

void		get_int_padding(t_arg *tg, char *arg_str, int sign)
{
	size_t digit_len;

	digit_len = ft_strlen(arg_str);
	if ((tg->precision && tg->precision <= digit_len) || tg->prec_flag)
	{
		tg->precision = 0;
		tg->lead_zeroes = 0;
		tg->prec_flag = 1;
	}
	else if (tg->precision > digit_len)
	{
		tg->precision = tg->precision - digit_len;
		if (LEFT)
			tg->lead_zeroes = 0;
	}
	if (MIN_WIDTH && MIN_WIDTH <= digit_len + sign + tg->precision)
		MIN_WIDTH = 0;
	if (MIN_WIDTH)
	{
		MIN_WIDTH = MIN_WIDTH - digit_len - sign - tg->precision;
		if (tg->precision || tg->prec_flag)
			tg->lead_zeroes = 0;
	}
	write_int(tg, &arg_str);
}
