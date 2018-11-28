/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 14:32:50 by ekelen            #+#    #+#             */
/*   Updated: 2016/12/10 14:11:32 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list *new;

	new = (t_list *)malloc(sizeof(t_list) * content_size);
	if (!new)
		return (NULL);
	if (!content)
	{
		(*new).content = NULL;
		(*new).content_size = 0;
	}
	else
	{
		if (!((*new).content = malloc(sizeof(content))))
			return (NULL);
		(*new).content = ft_memcpy((new->content), content, content_size);
		new->content_size = content_size;
	}
	(*new).next = NULL;
	return (new);
}
