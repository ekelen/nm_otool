/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 10:34:33 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 10:47:11 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

uint32_t	swap32(uint32_t x)
{
	x = ((x & 0x000000FF) << 24 |
				(x & 0x0000FF00) << 8 |
				(x & 0x00FF0000) >> 8 |
				(x & 0xFF000000) >> 24);
	return (x);
}

uint64_t	swap64(uint64_t x)
{
	x = ((x & 0x00000000000000FF) << 56 |
				(x & 0x000000000000FF00) << 40 |
				(x & 0x0000000000FF0000) << 24 |
				(x & 0x00000000FF000000) << 8 |
				(x & 0x000000FF00000000) >> 8 |
				(x & 0x0000FF0000000000) >> 24 |
				(x & 0x00FF000000000000) >> 40 |
				(x & 0xFF00000000000000) >> 56);
	return (x);
}

uint64_t	nswap64(uint64_t x)
{
	return (x);
}

uint32_t	nswap32(uint32_t x)
{
	return (x);
}