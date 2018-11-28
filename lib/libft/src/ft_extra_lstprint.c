/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extra_lstprint.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/10 14:51:59 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/10 15:56:46 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_extra_lstprint(t_list *lst)
{
	while (lst)
	{
		write(1, (char *)lst->content, sizeof(lst));
		write(1, "\n", 1);
		lst = lst->next;
	}
	lst = ((void *)0);
	return ;
}
