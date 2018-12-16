/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 14:31:29 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/16 13:26:43 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int	parse_magic(uint32_t magic, t_m *m)
{
	m->swap = (magic & CIGAM_MASK) == SWAP_MAGIC ? 1 : 0;
	m->swap32 = m->swap ? swap32 : nswap32;
	m->swap64 = m->swap ? swap64 : nswap64;
	if ((m->swap32(magic) & MAGIC_MASK) != MH_ANY)
		return (ERR_FILE);
	m->m64 = m->swap32(magic) & 1;
	m->offset = m->m64 ? MH_64_SIZE : MH_SIZE;
	return (SUCCESS);
}

int			init_mach_o(t_file *file, const void *data, size_t size, t_m *m)
{
	void		*mptr;
	uint32_t	magic;

	magic = 0x0000;
	m->symbols = NULL;
	m->st = NULL;
	m->next = NULL;
	m->data = (void *)data;
	m->nsects = 0x00000000;
	m->current_sect = 1;
	m->m64 = FALSE;
	if (!(m->end = ptr_check_msg(file->end, (void *)data + size, \
		0, "mach-o end")))
		return (ERR_FILE);
	if (!(mptr = ptr_check_msg(m->end, (void *)data, sizeof(uint32_t), \
		"magic number")))
		return (ERR_FILE);
	magic = *((uint32_t *)mptr);
	if (parse_magic(magic, m) > SUCCESS)
		return (ERR_FILE);
	if (get_segments(file, m) > SUCCESS)
		return (ERR_FILE);
	get_meta_print(file, m);
	return (SUCCESS);
}

int			add_mach(t_m **curr, t_m *new)
{
	if (!new)
		return (ERR_FILE);
	if (!*curr)
	{
		*curr = new;
		return (SUCCESS);
	}
	return (add_mach(&(*curr)->next, new));
}

void		remove_mach(t_m *m)
{
	if (m->symbols != NULL)
		free_symbols(m->symbols);
	free(m);
	return ;
}

void		free_machs(t_m *curr)
{
	t_m		*tmp;

	while (curr)
	{
		free_symbols(curr->symbols);
		curr->symbols = NULL;
		tmp = curr;
		curr = curr->next;
		free(tmp);
	}
	curr = NULL;
	return ;
}
