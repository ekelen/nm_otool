/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:00:17 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 21:03:21 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		weird_zeroes(t_arg *tg, char **arg_str)
{
	if (tg->prec_flag)
	{
		if (!MIN_WIDTH)
		{
			tg->len--;
			ft_strclr(*arg_str);
		}
		else
			*arg_str[0] = ' ';
	}
	return ;
}

void			write_int(t_arg *tg, char **arg_str)
{
	if (!ft_strcmp(*arg_str, "0"))
		weird_zeroes(tg, arg_str);
	if (tg->lead_zeroes && tg->num_prefix != 'E')
		ft_putchar_fd(tg->num_prefix, 1);
	if (!LEFT && MIN_WIDTH)
	{
		if (tg->lead_zeroes)
			write_padding_static('0', MIN_WIDTH);
		else
			write_padding_static(' ', MIN_WIDTH);
	}
	if (!tg->lead_zeroes && tg->num_prefix != 'E')
		ft_putchar_fd(tg->num_prefix, 1);
	write_padding_static('0', tg->precision);
	ft_putstr_fd(*arg_str, 1);
	if (LEFT)
		write_padding_static(' ', MIN_WIDTH);
}

static void		get_num_prefix(t_arg *tg, char *arg_str)
{
	int sign;

	sign = 0;
	if (tg->is_negative)
	{
		tg->lead_space = 0;
		tg->lead_plus = 0;
		tg->num_prefix = '-';
		sign = 1;
	}
	if (tg->lead_plus)
	{
		tg->num_prefix = '+';
		tg->lead_space = 0;
		sign = 1;
	}
	if (tg->lead_space)
	{
		tg->num_prefix = ' ';
		sign = 1;
	}
	get_int_padding(tg, arg_str, sign);
}

static void		get_int_len(t_arg *tg, char *arg_str)
{
	size_t arg_len;
	size_t numlen;

	numlen = ft_strlen(arg_str);
	arg_len = numlen;
	if (tg->lead_space || tg->lead_plus || tg->is_negative)
		arg_len += 1;
	if (tg->precision && tg->precision > numlen)
		arg_len += (tg->precision - numlen);
	if (MIN_WIDTH && MIN_WIDTH > arg_len)
		arg_len += (MIN_WIDTH - arg_len);
	tg->len = arg_len;
	get_num_prefix(tg, arg_str);
	return ;
}

void			handle_int(intmax_t arg_int, t_arg *tg)
{
	char *arg_str;

	if (!ft_strcmp(LN, "j"))
		arg_str = ft_itoa_abs((intmax_t)arg_int, tg);
	if (!ft_strcmp(LN, "ll"))
		arg_str = ft_itoa_abs((long long int)arg_int, tg);
	if (!ft_strcmp(LN, "l"))
		arg_str = ft_itoa_abs((long int)arg_int, tg);
	if (!ft_strcmp(LN, "h"))
		arg_str = ft_itoa_abs((short int)arg_int, tg);
	if (!ft_strcmp(LN, "hh"))
		arg_str = ft_itoa_abs((signed char)arg_int, tg);
	if (!ft_strcmp(LN, "z"))
		arg_str = ft_itoa_abs((ssize_t)arg_int, tg);
	if (!LN[0])
		arg_str = ft_itoa_abs((int)arg_int, tg);
	get_int_len(tg, arg_str);
	ft_strdel(&arg_str);
	return ;
}
