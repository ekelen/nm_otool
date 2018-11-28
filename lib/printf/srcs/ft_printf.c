/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 17:15:28 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 17:30:47 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		make_tg(t_arg *tg, char *s, va_list ap)
{
	if (ft_strchr(FLAGS, *s))
		decide_flags(tg, &s);
	while (ft_isdigit(*s))
		decide_width(tg, &s);
	if (*s == '.')
	{
		s++;
		decide_precision(tg, &s);
	}
	if (ft_strchr("hljz", *s))
		decide_len_mod(tg, &s);
	get_va_arg(tg, ap);
	return ;
}

static int		check_valid_form(char c)
{
	char valid_array[16];

	ft_strcpy(valid_array, TF);
	if (!ft_strchr(valid_array, c) || c == '\0')
		return (0);
	return (1);
}

static int		check_valid_arg(char c)
{
	char valid_array[20];

	ft_strcpy(valid_array, "0123456789hjzl#- .+");
	if (!ft_strchr(valid_array, c) || c == '\0')
		return (0);
	return (1);
}

static void		parse_initial(const char **format, va_list ap, t_arg *tg)
{
	size_t	arg_len;
	size_t	i;
	char	*s;

	arg_len = 0;
	i = 0;
	(*format)++;
	while (!ft_strchr(TF, *(*(format) + arg_len)) \
		&& check_valid_arg(*(*(format) + arg_len)))
		arg_len++;
	if (!check_valid_form(*(*(format) + arg_len)) || !(*(*(format) + arg_len)))
	{
		(*format) += arg_len - 1;
		return ;
	}
	tg->form = *(*(format) + arg_len);
	s = ft_strnew(arg_len + 1);
	while (++i < arg_len + 2)
		s[i - 1] = *((*format)++);
	(*format)--;
	make_tg(tg, s, ap);
	free(s);
	return ;
}

int				ft_printf(const char *format, ...)
{
	size_t	len;
	t_arg	*tg;
	va_list	ap;

	len = 0;
	tg = (t_arg *)malloc(sizeof(t_arg));
	va_start(ap, format);
	while (*format)
	{
		if (*format != '%' && *format)
		{
			write(1, (char *)format, 1);
			len++;
		}
		if (*format == '%')
		{
			zero_tg(tg);
			parse_initial(&format, ap, tg);
			len += tg->len;
		}
		format++;
	}
	free(tg);
	va_end(ap);
	return (len);
}
