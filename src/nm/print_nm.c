/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 21:22:29 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/06 14:13:01 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void check_display_2(const uint32_t flags, t_symbol *symbol, uint32_t *print)
{
	(void)symbol;
	if (flags & PRINT_PATHNAME)
		(*print) |= SHOW_PATHNAME;
    if (flags & SYM_NAME_ONLY)
        (*print) &= (~SHOW_VAL_COL & ~SHOW_TYPE);
}

static uint32_t check_display(const uint32_t flags, t_symbol *symbol)
{
	uint32_t print;

	print = SHOW_DEFAULT;
    if (symbol->n_type & N_STAB && !(flags & ALL))
        print &= ~SHOW_ANY;

    if (symbol->type == 'I')
        print &= ~SHOW_VALUE;

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
	check_display_2(flags, symbol, &print);
    return (print);
}

static void print_symbol_value(t_symbol *s, uint32_t print)
{
	if ((print & SHOW_VALUE) == SHOW_VALUE)
		ft_printf(s->m64 ? "%016llx " : "%08llx ", s->n_value);
	else
		ft_printf(s->m64 ? "%016c " : "%08c ", ' ');
	return;
}

static void print_pathname(t_file *file, t_mach_o *m, t_symbol *head, t_symbol *current)
{
	(void)head;
	(void)current;
	if (file->info & IS_SINGLE_MACH || !(file->info & IS_MULTI) \
		|| ((file->info & IS_FAT) && !(m->arch.arch_info.name[0])))
		ft_printf("%s: ", file->filename);
	else if (file->info & IS_STATLIB)
		ft_printf("%s:%s: ", file->filename, m->ofile.name);
	else if (file->info & IS_FAT)
		ft_printf("(for architecture %s):%s: ", m->arch.arch_info.name, file->filename);
}

static void print_in_order(t_file *file, t_mach_o *m, t_symbol *head, t_symbol *current)
{
	uint32_t print;

    if (!current)
        return;
    print_in_order(file, m, head, current->left);
	print = check_display(file->flags, current);
	if (print & SHOW_ANY)
	{
		if (print & SHOW_PATHNAME)
			print_pathname(file, m, head, current);
		if (print & SHOW_VAL_COL)
			print_symbol_value(current, print);
		if (print & SHOW_TYPE)
    		ft_printf("%c ", current->type);
    	ft_printf("%s", current->nom);
		if (current->type == 'I')
			ft_printf(" (indirect for %s)", current->nom);
		ft_putendl("");
	}
    print_in_order(file, m, head, current->right);
}

void print_machs(t_file *file, t_mach_o *m)
{
	if (!m)
        return;
    else if (m)
    {
		m->print_meta(file, m);
        print_in_order(file, m, m->symbols, m->symbols);
		free_symbols(m->symbols);
		m->symbols = NULL;
    }
    print_machs(file, m->next);
}