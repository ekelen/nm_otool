/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 13:19:58 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/09 11:39:04 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	*ptr_check(const void *end, void *req, size_t len)
{
	if ((uint8_t *)req + len > (uint8_t *)end)
		return (NULL);
	return (req);
}

void	*ptr_check_msg(const void *end, void *req, size_t len, const char *msg)
{
	if (ptr_check(end, req, len) != NULL)
		return (req);
	ft_putstr_upper_fd(msg, 2);
	ft_putstr_fd(" extends past allocated space.\n", 2);
	return (NULL);
}

void	free_symbols(t_sym *curr)
{
	if (!curr)
		return ;
	free_symbols(curr->left);
	free_symbols(curr->right);
	free((void*)curr);
}
