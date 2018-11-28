/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:21:12 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 21:23:02 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t		get_wstr_len(wchar_t *arg_wstr, size_t *s_len, t_arg *tg)
{
	size_t wchar_len;

	wchar_len = 0;
	while (arg_wstr[*s_len])
	{
		if (arg_wstr[*s_len] <= 0x7F)
			wchar_len += 1;
		else if (arg_wstr[*s_len] <= 0x7FF)
			wchar_len += 2;
		else if (arg_wstr[*s_len] <= 0xFFFF)
			wchar_len += 3;
		else
			wchar_len += 4;
		(*s_len)++;
	}
	if (tg->precision && tg->precision < *s_len)
		*s_len = tg->precision;
	return (wchar_len);
}

static size_t		get_num_wspaces(size_t ret_len, t_arg *tg)
{
	size_t num_spaces;

	num_spaces = 0;
	if (MIN_WIDTH)
	{
		if (MIN_WIDTH > ret_len)
			num_spaces = MIN_WIDTH - ret_len;
		else
			num_spaces = 0;
	}
	return (num_spaces);
}

static void			handle_wstr(wchar_t *arg_wstr, t_arg *tg)
{
	size_t i;
	size_t num_spaces;
	size_t str_len;
	size_t ret_len;

	i = 0;
	str_len = 0;
	ret_len = get_wstr_len(arg_wstr, &str_len, tg);
	num_spaces = get_num_wspaces(ret_len, tg);
	if (LEFT)
	{
		while (i++ < str_len + 1)
			handle_wchar((int)arg_wstr[i - 1], tg);
		write_padding_static(' ', num_spaces);
	}
	else
	{
		write_padding_static(' ', num_spaces);
		while (i++ < str_len + num_spaces + 1)
			handle_wchar((int)arg_wstr[i - 1], tg);
	}
	tg->len = ret_len + num_spaces;
}

void				handle_wstr_helper(wchar_t *arg_wstr, t_arg *tg)
{
	if (!arg_wstr)
	{
		handle_str(NULL, tg);
		return ;
	}
	else
		handle_wstr(arg_wstr, tg);
}
