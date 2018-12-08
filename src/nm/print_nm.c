/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 21:22:29 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 13:48:17 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static uint32_t		check_display(const uint32_t flags, t_sym *symbol)
{
	uint32_t print;

	print = SHOW_DEFAULT;
	if (symbol->n_type & N_STAB && !(flags & ALL))
		print &= ~SHOW_ANY;
	if (!symbol->n_value && ft_strchr("Uu", symbol->type))
		print &= ~SHOW_VALUE;
	if (flags & UNDEF_ONLY)
	{
		if (!ft_strchr("uU", symbol->type))
			print &= ~SHOW_ANY;
		else
			print &= (~SHOW_VAL_COL & ~SHOW_TYPE);
	}
	if (flags & NO_UNDEF)
		if (ft_strchr("uU", symbol->type))
			print &= ~SHOW_ANY;
	if (flags & PRINT_PATHNAME)
		(print) |= SHOW_PATHNAME;
	if (flags & SYM_NAME_ONLY)
		(print) &= (~SHOW_VAL_COL & ~SHOW_TYPE);
	return (print);
}

static void			print_symbol_value(t_sym *s, uint32_t print)
{
	if ((print & SHOW_VALUE) == SHOW_VALUE)
		ft_printf(s->m64 ? "%016llx " : "%08llx ", s->n_value);
	else
		ft_printf(s->m64 ? "%016c " : "%08c ", ' ');
	return ;
}

static void			print_pathname(t_file *file, t_m *m)
{
	if ((file->info & IS_SINGLE_MACH) || !(file->info & IS_MULTI) \
		|| ((file->info & IS_FAT) && !(m->arch.arch_info.name[0])))
		ft_printf("%s: ", file->filename);
	else if (file->info & IS_STATLIB)
		ft_printf("%s:%s: ", file->filename, m->ofile.name);
	else if (file->info & IS_FAT)
		ft_printf("(for architecture %s):%s: ", m->arch.arch_info.name, \
		file->filename);
}

static void			print_syms(t_file *file, t_m *m, t_sym *hd, t_sym *curr)
{
	uint32_t print;

	if (!curr)
		return ;
	print_syms(file, m, hd, curr->left);
	print = check_display(file->flags, curr);
	if (print & SHOW_ANY)
	{
		if (print & SHOW_PATHNAME)
			print_pathname(file, m);
		if (print & SHOW_VAL_COL)
			print_symbol_value(curr, print);
		if (print & SHOW_TYPE)
			ft_printf("%c ", curr->type);
		ft_putendl(curr->nom);
	}
	print_syms(file, m, hd, curr->right);
}

void				print_machs(t_file *file, t_m *m)
{
	if (!m)
		return ;
	else if (m)
	{
		m->print_meta(file, m);
		print_syms(file, m, m->symbols, m->symbols);
		free_symbols(m->symbols);
		m->symbols = NULL;
	}
	print_machs(file, m->next);
}
