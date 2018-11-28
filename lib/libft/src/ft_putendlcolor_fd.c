/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 09:41:08 by ekelen            #+#    #+#             */
/*   Updated: 2017/03/13 12:38:14 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		get_color(char const *color, int fd)
{
	if (!(ft_strcmp(color, "red")))
		ft_putstr_fd("\033[31m", fd);
	else if (!(ft_strcmp(color, "yellow")))
		ft_putstr_fd("\033[33m", fd);
	else if (!(ft_strcmp(color, "green")))
		ft_putstr_fd("\033[32m", fd);
	else if (!(ft_strcmp(color, "lightgreen")))
		ft_putstr_fd("\033[92m", fd);
	else if (!(ft_strcmp(color, "bold")))
		ft_putstr_fd("\033[1;97m", fd);
	else if (!(ft_strcmp(color, "cyan")))
		ft_putstr_fd("\033[96m", fd);
	else if (!(ft_strcmp(color, "grey")))
		ft_putstr_fd("\033[37m", fd);
	return (1);
}

void	ft_putendlcolor_fd(char const *s, int fd, char *color, int nl)
{
	int i;

	i = 0;
	get_color(color, fd);
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	ft_putstr_fd("\033[0m", fd);
	if (nl)
		write(fd, "\n", 1);
	return ;
}
