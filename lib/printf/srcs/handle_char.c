/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 20:59:18 by ekelen            #+#    #+#             */
/*   Updated: 2017/03/18 16:22:02 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		weird_czeroes(t_arg *tg)
{
	if (!LEFT && tg->lead_zeroes)
		write_padding('0', tg, MIN_WIDTH - 1);
	else
		write_padding(' ', tg, MIN_WIDTH - 1);
	write(1, "\0", 1);
	tg->len++;
	return ;
}

static void		format_char(int c, t_arg *tg)
{
	size_t num_spaces;

	num_spaces = MIN_WIDTH ? MIN_WIDTH - 1 : 0;
	if (!c && num_spaces)
	{
		weird_czeroes(tg);
		return ;
	}
	if (!LEFT)
	{
		if (num_spaces)
			write_padding(' ', tg, num_spaces);
	}
	if (c)
		ft_putchar_fd((unsigned char)c, 1);
	tg->len++;
	if (LEFT)
	{
		write_padding(' ', tg, num_spaces);
	}
	return ;
}

void			handle_char(int c, t_arg *tg)
{
	if ((tg->form == 'C' || !ft_strcmp(LN, "l")))
	{
		locale_check(c, tg);
		return ;
	}
	if (!MIN_WIDTH && !LN[0])
	{
		ft_putchar_fd((unsigned char)c, 1);
		tg->len = 1;
		return ;
	}
	format_char(c, tg);
	return ;
}
