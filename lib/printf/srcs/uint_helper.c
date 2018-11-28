/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uint_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:24:40 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 21:25:01 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	get_uint_lenmod(t_arg *tg)
{
	if (tg->form == 'U' || tg->form == 'p')
	{
		if (tg->form == 'p')
			tg->prefix = 1;
		ft_strcpy(LN, "l");
	}
	if (tg->form == 'O' || tg->form == 'D')
		ft_strcpy(LN, "l");
	return ;
}

void	format_hex_zero(t_arg *tg, char **arg_str)
{
	if (tg->base == 16)
	{
		if (tg->prec_flag && !MIN_WIDTH)
		{
			tg->len--;
			ft_strclr(*arg_str);
		}
		else if (tg->prec_flag && MIN_WIDTH)
			*arg_str[0] = ' ';
	}
	if (tg->form != 'p')
		tg->prefix = 0;
	return ;
}

void	format_uint_zero(t_arg *tg, char **arg_str)
{
	if (!tg->prefix && tg->prec_flag)
	{
		if (!MIN_WIDTH)
		{
			tg->len--;
			ft_strclr(*arg_str);
		}
		else
			*arg_str[0] = ' ';
	}
	tg->prefix = 0;
	return ;
}
