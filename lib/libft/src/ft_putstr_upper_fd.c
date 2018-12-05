/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 19:50:35 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/05 13:31:42 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_putstr_upper_fd(const char *s, int fd)
{
	size_t i;

	i = -1;
	while (++i < ft_strlen(s))
		ft_putchar_fd(ft_toupper(s[i]), fd);
	return;
}