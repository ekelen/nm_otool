/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 11:13:21 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/09 11:35:06 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static char	parse_section_type(uint64_t nsects, uint8_t n_sect)
{
	if (n_sect == NO_SECT)
		return ('u');
	else if (n_sect <= MAX_SECT)
	{
		if (n_sect == (nsects & TEXT_SECT))
			return ('t');
		else if (n_sect == (nsects & DATA_SECT) >> 8)
			return ('d');
		else if (n_sect == (nsects & BSS_SECT) >> 16)
			return ('b');
		else
			return ('s');
	}
	return ('?');
}

static char	parse_type(uint64_t nsects, t_sym *symbol)
{
	static int	i = 0;
	uint8_t		c;

	c = symbol->n_type;
	if (c & N_STAB)
		return ('-');
	c = c & N_TYPE;
	if (c == N_UNDF)
		c = symbol->n_value == 0 ? 'u' : 'c';
	else if (c == N_PBUD)
		c = 'u';
	else if (c == N_ABS)
		c = 'a';
	else if (c == N_SECT)
		c = parse_section_type(nsects, symbol->n_sect);
	else if (c == N_INDR)
		c = 'i';
	else
		c = '?';
	if (symbol->n_type & N_EXT)
		c -= 32;
	i = i + 1;
	return (unsigned char)c;
}

static char	*get_sym_name(t_m *m, struct symtab_command *st, t_u_nl nl)
{
	void		*name_start;
	void		*strtab_end;
	uint32_t	n_strx;

	n_strx = m->m64 ? m->swap32(nl.nl64->n_un.n_strx)
		: m->swap32(nl.nl32->n_un.n_strx);
	name_start = (void *)m->data
		+ st->stroff
		+ n_strx;
	strtab_end = (void *)m->data
		+ st->stroff
		+ st->strsize;
	return ((char *)ptr_check_msg(strtab_end, (void *)name_start, \
		0, "symbol name"));
}

static int	fill_symbol_data(t_m *m, struct symtab_command *st, t_sym *s)
{
	t_u_nl nl;

	s->m64 = m->m64;
	if (s->m64)
		nl.nl64 = (struct nlist_64 *)(s->nptr);
	else
		nl.nl32 = (struct nlist *)(s->nptr);
	if (!(s->nom = get_sym_name(m, st, nl)))
		return (ERR_FILE);
	s->n_value = s->m64 ? m->swap64(nl.nl64->n_value)
		: (uint64_t)(m->swap32(nl.nl32->n_value));
	s->n_sect = s->m64 ? nl.nl64->n_sect : nl.nl32->n_sect;
	s->n_type = s->m64 ? nl.nl64->n_type : nl.nl32->n_type;
	s->type = parse_type(m->nsects, s);
	return (SUCCESS);
}

int			add_symbol(t_file *file, t_m *m, struct symtab_command *st, \
	const void *nptr)
{
	t_sym		*s;
	t_status	res;

	if (!(s = (t_sym *)malloc(sizeof(t_sym))))
		return (ERR_ALLOCATION);
	s->nptr = nptr;
	s->left = NULL;
	s->right = NULL;
	if ((res = fill_symbol_data(m, st, s)) > SUCCESS)
	{
		free(s);
		return (res);
	}
	sort_symbol(file->sort, &(m->symbols), s, (file->flags & SORT_REVERSE));
	return (SUCCESS);
}
