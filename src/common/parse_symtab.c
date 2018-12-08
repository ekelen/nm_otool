/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_symtab.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 11:34:24 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 11:37:09 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int	parse_symtab_2(t_file *file, t_m *m, t_symtab_command *st)
{
	uint32_t	i;
	void		*ptr;

	ptr = m->m64 
		? ptr_check_msg(m->end, (void *)m->data + st->symoff, NLIST_64_SIZE * st->nsyms, "nlists 64")
		: ptr_check_msg(m->end, (void *)m->data + st->symoff, NLIST_SIZE * st->nsyms, "nlists");
	i = -1;
	while (++i < st->nsyms)
	{
		if (i > 0)
			ptr = m->m64
				? ptr_check_msg(m->end, ptr + NLIST_64_SIZE, NLIST_64_SIZE, "next nlist 64")
				: ptr_check_msg(m->end, ptr + NLIST_SIZE, NLIST_SIZE, "next nlist");
		if (!ptr)
			return (ERR_FILE);
		if ((add_symbol(file, m, st, ptr)) > SUCCESS)
			return (ERR_FILE);
	}
	return (SUCCESS);
}

int			parse_symtab(t_file *file, t_m *m, const struct load_command *cmd)
{
	t_symtab_command	*st;
	t_symtab_command	st2;
	void				*strtab;

	if (!(st = (t_symtab_command *)ptr_check_msg(m->end, (void *)cmd, \
		SYMTAB_SIZE, "symtab command")))
		return (ERR_FILE);
	st2.strsize = m->swap32(st->strsize);
	st2.stroff = m->swap32(st->stroff);
	st2.symoff = m->swap32(st->symoff);
	st2.nsyms = m->swap32(st->nsyms);
	if (!(strtab = ptr_check_msg(m->end, (void *)m->data + st2.stroff, \
		st2.strsize, "strtab")))
		return (ERR_FILE);
	return (parse_symtab_2(file, m, &st2));
}
