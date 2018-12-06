/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 13:08:02 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/05 16:56:30 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static int	ft_numlen_base(intmax_t nbr, int base)
// {
// 	int num_len;

//     num_len = nbr < 0 ? 1 : 0;
//     nbr = ft_abs(nbr);
// 	while (nbr != 0)
// 	{
// 		nbr /= base;
// 		num_len++;
// 	}
// 	return (num_len);
// }

char		*ft_itoa_base(intmax_t nbr, int base)
{
	char	*int_str;
	int		n;
	int		i;
	char	tab[17];

	if (nbr == 0)
		return (ft_strdup("0"));
	ft_strcpy(tab, "0123456789abcdef");
	n = ft_numlen_base(nbr, base, nbr < 0);
	int_str = (char *)malloc(sizeof(char) * n + 1);
    i = 0;
	if (nbr < 0)
        int_str[0] = '-';
	while (nbr != 0)
	{
		int_str[n - i] = tab[nbr % base];
		nbr /= base;
		i++;
	}
	int_str[n] = '\0';
	return (int_str);
}
