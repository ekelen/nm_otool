/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 13:08:02 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/07 12:45:01 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
