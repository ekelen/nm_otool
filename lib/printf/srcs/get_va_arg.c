/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_va_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 20:55:09 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 20:58:18 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		assign_base(t_arg *tg, char c)
{
	if (ft_strchr("oO", c))
		tg->base = 8;
	else if (ft_strchr("xXp", c))
		tg->base = 16;
	return ;
}

void			get_va_arg(t_arg *tg, va_list ap)
{
	char c;

	c = tg->form;
	if (c == 'd' || c == 'i')
		handle_int(va_arg(ap, intmax_t), tg);
	if (ft_strchr("uUDoOxXp", c))
	{
		assign_base(tg, c);
		handle_uint(va_arg(ap, uintmax_t), tg);
	}
	if (c == 'c' || c == 'C')
		handle_char(va_arg(ap, int), tg);
	if ((c == 'S' && MB_CUR_MAX > 1) || (c == 's' && LN[0] == 'l'))
		handle_wstr_helper(va_arg(ap, wchar_t *), tg);
	else if (c == 's' || c == 'S')
	{
		handle_str(va_arg(ap, char *), tg);
		return ;
	}
	if (c == '%')
		handle_percent(c, tg);
	return ;
}
