/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raw_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:23:37 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 21:23:57 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		zero_tg(t_arg *tg)
{
	tg->lead_zeroes = 0;
	LEFT = 0;
	tg->prefix = 0;
	tg->lead_space = 0;
	tg->lead_plus = 0;
	tg->is_negative = 0;
	MIN_WIDTH = 0;
	tg->precision = 0;
	tg->prec_flag = 0;
	ftek_strnclr(LN, 3);
	tg->form = '0';
	tg->num_prefix = 'E';
	tg->base = 10;
	tg->len = 0;
	return ;
}

void		decide_flags(t_arg *tg, char **s)
{
	while (ft_strchr(FLAGS, **s))
	{
		if (**s == '0')
			tg->lead_zeroes = 1;
		if (**s == '-')
			LEFT = 1;
		if (**s == '#')
			tg->prefix = 1;
		if (**s == ' ')
			tg->lead_space = 1;
		if (**s == '+')
			tg->lead_plus = 1;
		(*s)++;
	}
	return ;
}

void		decide_width(t_arg *tg, char **s)
{
	int width;
	int	dec_place;

	dec_place = 10;
	width = **s - '0';
	(*s)++;
	while (ft_isdigit(**s))
	{
		width *= 10;
		width += **s - '0';
		(*s)++;
	}
	MIN_WIDTH = width;
	return ;
}

void		decide_precision(t_arg *tg, char **s)
{
	int precision;
	int	dec_place;

	dec_place = 10;
	precision = **s - '0';
	if (!ft_isdigit(**s))
	{
		tg->precision = 0;
		tg->prec_flag = 1;
		return ;
	}
	(*s)++;
	while (ft_isdigit(**s))
	{
		precision *= 10;
		precision += **s - '0';
		(*s)++;
	}
	tg->precision = precision;
	if (!tg->precision)
		tg->prec_flag = 1;
	return ;
}

void		decide_len_mod(t_arg *tg, char **s)
{
	LN[0] = **s;
	if (*(*s + 1) == 'h')
	{
		LN[1] = 'h';
		(*s) += 2;
		return ;
	}
	else if (*(*s + 1) == 'l')
	{
		LN[1] = 'l';
		(*s) += 2;
		return ;
	}
	(*s)++;
	return ;
}
