/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_uint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:13:08 by ekelen            #+#    #+#             */
/*   Updated: 2018/10/19 13:52:57 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		write_base_prefix(t_arg *tg)
{
	if (tg->prefix)
	{
		write(1, "0", 1);
		if (tg->prefix == 2)
			ft_putchar_fd((tg->form == 'X' ? 'X' : 'x'), 1);
	}
	return ;
}

static void		write_uint(t_arg *tg, char **arg_str)
{
	if (!LEFT)
	{
		if (!tg->lead_zeroes)
			write_padding_static(' ', MIN_WIDTH);
		write_base_prefix(tg);
		if (tg->lead_zeroes)
			write_padding_static('0', MIN_WIDTH);
	}
	else
		write_base_prefix(tg);
	write_padding_static('0', tg->precision);
	ft_putstr_fd(*arg_str, 1);
	if (LEFT)
		write_padding_static(' ', MIN_WIDTH);
}

static void		cleanup_uint(t_arg *tg, char *arg_str, size_t digit_len)
{
	if (tg->precision)
	{
		if (tg->precision > digit_len)
			tg->precision = (tg->precision - digit_len);
		else if (tg->precision <= digit_len)
		{
			tg->precision = 0;
			tg->prec_flag = 1;
		}
		if (tg->precision || LEFT)
			tg->lead_zeroes = 0;
	}
	if (MIN_WIDTH && MIN_WIDTH <= digit_len + tg->prefix + tg->precision)
		MIN_WIDTH = 0;
	if (MIN_WIDTH)
		MIN_WIDTH = MIN_WIDTH - digit_len - tg->prefix - tg->precision;
	if (!ft_strcmp("0", arg_str))
		tg->base == 16 ? format_hex_zero(tg, &arg_str) : \
			format_uint_zero(tg, &arg_str);
	write_uint(tg, &arg_str);
	return ;
}

static void		get_uint_len(t_arg *tg, char *arg_str)
{
	size_t arg_len;
	size_t digit_len;

	digit_len = ft_strlen(arg_str);
	arg_len = digit_len;
	if (tg->prefix && (ft_strcmp("0", arg_str) || tg->form == 'p'))
	{
		arg_len += (ft_strchr("oO", tg->form) ? 1 : 2);
		tg->prefix += (ft_strchr("oO", tg->form) ? 0 : 1);
	}
	if (tg->precision && tg->precision > digit_len)
		arg_len += (tg->precision - digit_len);
	if (MIN_WIDTH && MIN_WIDTH > arg_len)
		arg_len += (MIN_WIDTH - arg_len);
	tg->len = arg_len;
	cleanup_uint(tg, arg_str, digit_len);
	ft_strdel(&arg_str);
	return ;
}

void			handle_uint(uintmax_t arg_uint, t_arg *tg)
{
	char *arg_uint_str;

	get_uint_lenmod(tg);
	if (!ft_strcmp(LN, "l"))
		arg_uint_str = ft_f_itoa_base((unsigned long int)arg_uint, tg->base, tg);
	else if (!ft_strcmp(LN, "ll"))
		arg_uint_str = ft_f_itoa_base((unsigned long long)arg_uint, tg->base, tg);
	else if (!ft_strcmp(LN, "h"))
		arg_uint_str = ft_f_itoa_base((unsigned short)arg_uint, tg->base, tg);
	if (!ft_strcmp(LN, "hh"))
		arg_uint_str = ft_f_itoa_base((unsigned char)arg_uint, tg->base, tg);
	if (!ft_strcmp(LN, "z"))
		arg_uint_str = ft_f_itoa_base((size_t)arg_uint, tg->base, tg);
	if (!ft_strcmp(LN, "j"))
		arg_uint_str = ft_f_itoa_base((uintmax_t)arg_uint, tg->base, tg);
	if (!LN[0])
		arg_uint_str = ft_f_itoa_base((unsigned int)arg_uint, tg->base, tg);
	get_uint_len(tg, arg_uint_str);
}
