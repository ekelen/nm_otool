/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wchar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:19:30 by ekelen            #+#    #+#             */
/*   Updated: 2018/10/19 13:58:47 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int		get_wchar_len(wchar_t w)
{
	if (w <= 0x7F)
		return (1);
	else if (w <= 0x7FF)
		return (2);
	else if (w <= 0xFFFF)
		return (3);
	else
		return (4);
}

static void		print_wchar_s(char **binary_wchar)
{
	int i;
	int wchar_piece;

	i = 0;
	while (binary_wchar[i])
	{
		wchar_piece = ft_f_atoi_base(binary_wchar[i], 2);
		if (wchar_piece)
			write(1, &wchar_piece, 1);
		free(binary_wchar[i]);
		i++;
	}
	return ;
}

void			locale_check(int c, t_arg *tg)
{
	if (MB_CUR_MAX != 1)
	{
		handle_wchar(c, tg);
		return ;
	}
	else
	{
		if (c > 255)
		{
			tg->len = -1;
			return ;
		}
		else
		{
			ft_putchar_fd((unsigned char)c, 1);
			tg->len = 1;
		}
	}
}

void			handle_wchar(int c, t_arg *tg)
{
	int		wchar_len;
	char	**binary_wchar;
	wchar_t	w;
	int		i;

	i = 0;
	w = (wchar_t)c;
	wchar_len = get_wchar_len(w);
	tg->is_negative = 0;
	binary_wchar = ft_itobyte(c, wchar_len);
	tg->len = wchar_len;
	print_wchar_s(binary_wchar);
	free(binary_wchar);
	return ;
}
