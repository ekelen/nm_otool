/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:04:42 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 21:09:45 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	get_strlen(char *arg_str, t_arg *tg)
{
	size_t str_len;
	size_t i;

	i = 0;
	str_len = ft_strlen(arg_str);
	if (tg->precision < str_len)
	{
		if (tg->precision || tg->prec_flag)
			str_len = tg->precision;
	}
	return (str_len);
}

static void		format_str(char *arg_str, t_arg *tg)
{
	size_t str_len;
	size_t num_spaces;
	size_t i;

	i = 0;
	str_len = get_strlen(arg_str, tg);
	num_spaces = MIN_WIDTH > str_len ? MIN_WIDTH - str_len : 0;
	if (!LEFT)
		write_padding(' ', tg, num_spaces);
	while (i < str_len)
	{
		ft_putchar_fd(*(arg_str + i), 1);
		i++;
	}
	tg->len += i;
	if (LEFT)
		write_padding(' ', tg, num_spaces);
	return ;
}

void			handle_str(char *arg_str, t_arg *tg)
{
	if (arg_str)
		format_str(arg_str, tg);
	else
	{
		ft_putstr_fd("(null)", 1);
		tg->len = 6;
	}
	return ;
}
