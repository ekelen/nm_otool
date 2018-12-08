/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstrcolon_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 10:40:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/07 12:44:19 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Print string followed by colon followed by end character
*/

void	ft_putstrcolon_fd(const char *s, int fd, char end)
{
	ft_putstr_fd(s, fd);
	ft_putchar_fd(':', fd);
	ft_putchar_fd(end, fd);
	return ;
}
