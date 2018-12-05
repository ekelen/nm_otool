/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstrcolon_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 10:40:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/05 10:45:48 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Handy for interpolating error messages
*/
void ft_putstrcolon_fd(const char *s, int fd, char end)
{
    ft_putstr_fd(s, fd);
    ft_putchar_fd(':', fd);
    ft_putchar_fd(end, fd);
	return;
}