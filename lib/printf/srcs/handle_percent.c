/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_percent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:03:41 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 21:04:07 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		handle_percent(char percent_char, t_arg *tg)
{
	size_t num_spaces;
	size_t i;

	i = 0;
	num_spaces = MIN_WIDTH ? MIN_WIDTH - 1 : 0;
	if (!LEFT)
		write_padding(' ', tg, num_spaces);
	ft_putchar_fd((char)percent_char, 1);
	tg->len++;
	if (LEFT)
		write_padding(' ', tg, num_spaces);
	return ;
}
