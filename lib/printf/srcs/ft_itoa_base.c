/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 16:19:17 by ekelen            #+#    #+#             */
/*   Updated: 2018/10/19 13:51:49 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char		hex_upper(char c, t_arg *tg)
{
	if (tg->form == 'X')
	{
		if (c >= 97 && c <= 122)
			c -= 32;
	}
	return (c);
}

static int	ft_numlen_base(uintmax_t nbr, int base)
{
	int num_len;

	num_len = 0;
	while (nbr != 0)
	{
		nbr /= base;
		num_len++;
	}
	return (num_len);
}

char		*ft_f_itoa_base(uintmax_t nbr, int base, t_arg *tg)
{
	char	*int_str;
	int		n;
	int		i;
	char	tab[17];

	tg->is_negative = 0;
	if (nbr == 0)
	{
		return (ft_strdup("0"));
	}
	ft_strcpy(tab, "0123456789abcdef");
	n = ft_numlen_base(nbr, base);
	int_str = malloc(sizeof(char) * n + 1);
	i = 1;
	while (nbr != 0)
	{
		int_str[n - i] = hex_upper(tab[nbr % base], tg);
		nbr /= base;
		i++;
	}
	int_str[n] = '\0';
	return (int_str);
}
