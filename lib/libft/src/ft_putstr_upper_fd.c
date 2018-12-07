/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_upper_fd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 19:50:35 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/07 12:42:32 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_putstr_upper_fd(const char *s, int fd)
{
	size_t i;

	i = -1;
	while (++i < ft_strlen(s))
		ft_putchar_fd(ft_toupper(s[i]), fd);
	return ;
}
