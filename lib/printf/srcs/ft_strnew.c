/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/01 17:55:52 by ekelen            #+#    #+#             */
/*   Updated: 2017/02/20 15:41:29 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_strnew(size_t size)
{
	char *new_str;

	new_str = (char *)malloc(sizeof(char *) * size + 1);
	if (!new_str)
		return (NULL);
	ft_memset(new_str, (int)'\0', size + 1);
	return (new_str);
}
