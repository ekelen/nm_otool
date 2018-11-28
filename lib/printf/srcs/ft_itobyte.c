/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 20:16:34 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 17:14:45 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#define WCHAR_MASK_1 "0xxxxxxx"
#define WCHAR_MASK_2 "110xxxxx 10xxxxxx"
#define WCHAR_MASK_3 "1110xxxx 10xxxxxx 10xxxxxx"
#define WCHAR_MASK_4 "11110xxx 10xxxxxx 10xxxxxx 10xxxxxx"

static char		*which_mask(int wchar_len)
{
	char *mask;

	mask = NULL;
	if (wchar_len)
	{
		if (wchar_len == 1)
			mask = ft_strdup(WCHAR_MASK_1);
		else if (wchar_len == 2)
			mask = ft_strdup(WCHAR_MASK_2);
		else if (wchar_len == 3)
			mask = ft_strdup(WCHAR_MASK_3);
		else if (wchar_len == 4)
			mask = ft_strdup(WCHAR_MASK_4);
		return (mask);
	}
	return (mask);
}

static char		**apply_mask(char *bin, int wchar_len)
{
	int		lnbin;
	int		lnmsk;
	char	*mask;
	char	**split_test;

	mask = which_mask(wchar_len);
	lnbin = ft_strlen(bin) - 1;
	lnmsk = ft_strlen(mask) - 1;
	while (lnmsk >= 0 && lnbin >= 0)
	{
		if (mask[lnmsk] == 'x')
			mask[lnmsk] = bin[lnbin--];
		lnmsk--;
	}
	lnbin = 0;
	while (lnbin < (int)ft_strlen(mask))
	{
		if (mask[lnbin] == 'x')
			mask[lnbin] = '0';
		lnbin++;
	}
	split_test = ft_strsplit(mask, ' ');
	free(mask);
	return (split_test);
}

static int		ft_bin_numlen(unsigned int b)
{
	int ret;

	ret = 0;
	while (b != 0)
	{
		b /= 2;
		ret++;
	}
	return (ret);
}

char			**ft_itobyte(unsigned int b, char wchar_len)
{
	char	*byte_str;
	char	**all_bytes;
	int		n;
	int		i;

	if (b == 0)
		byte_str = ft_strdup("0");
	else
	{
		n = ft_bin_numlen(b);
		byte_str = malloc(sizeof(char) * n + 1);
		i = 1;
		while (b != 0)
		{
			byte_str[n - i++] = (b % 2) + '0';
			b /= 2;
		}
		byte_str[n] = '\0';
	}
	all_bytes = apply_mask(byte_str, wchar_len);
	ft_strdel(&byte_str);
	return (all_bytes);
}
