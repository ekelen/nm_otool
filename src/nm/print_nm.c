/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 21:22:29 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/28 21:41:31 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>
#include <assert.h>

// TODO: do something with this landfill
static uint32_t check_print(uint32_t flags, t_symbol *symbol)
{
	uint32_t print;

	print = 0x000000;
    print |= (SHOW_ANY | SHOW_TYPE | SHOW_NAME | SHOW_VAL_COL | SHOW_VALUE);
    if ((flags & 1) & ALL)
    {
        print |= (SHOW_ANY | SHOW_TYPE | SHOW_NAME | SHOW_VAL_COL | SHOW_VALUE);
        
    }
    else if (symbol->n_type & N_STAB)
    {
        print &= ~SHOW_ANY;
    }
    else
    {
        print |= (SHOW_ANY | SHOW_TYPE | SHOW_NAME | SHOW_VAL_COL | SHOW_VALUE);
    }

    if (symbol->type == 'I')
    {
        print &= ~SHOW_VALUE;
    }

    if (!symbol->n_value && ft_strchr("Uu", symbol->type))
    {
        print &= ~SHOW_VALUE;
    }

    if ((flags & 4) & UNDEF_ONLY)
    {
        if (!ft_strchr("uU", symbol->type))
        {
            print &= ~SHOW_ANY;
        } else {
            print &= ~SHOW_VAL_COL;
        }
    }
    if ((flags & 8) & NO_UNDEF)
        if (ft_strchr("uU", symbol->type))
            print &= ~SHOW_ANY;
    return print;
}

static void print_symbol(t_symbol *s)
{
	if ((s->print & 1) & SHOW_VALUE)
		ft_printf(s->m64 ? "%016llx " : "%08llx ", s->n_value);
	else
		ft_printf(s->m64 ? "%016c " : "%08c ", ' ');
	return;
}

static void print_in_order(uint32_t flags, t_symbol *head, t_symbol *current)
{
    if (!current)
        return;
    print_in_order(flags, head, current->left);
	current->print = check_print(flags, current);
	if ((current->print & SHOW_ANY_MASK) & SHOW_ANY)
	{
		if ((current->print & 16) & SHOW_VAL_COL)
			print_symbol(current);
		if ((current->print & 2) & SHOW_TYPE)
    		ft_printf("%c ", current->type);
    	ft_printf("%s", current->nom);
		if (current->type == 'I')
			ft_printf(" (indirect for %s)", current->nom);
		ft_putendl("");
	}
    print_in_order(flags, head, current->right);
}

void print_meta_fat(t_file *file, t_mach_o *m)
{
	ft_putendl("");
    ft_printf("%s (for architecture %s):\n", file->filename, m->arch.arch_info.name);
}

void print_meta_statlib(t_file *file, t_mach_o *m)
{
	ft_putendl("");
    ft_printf("%s(%s):\n", file->filename, m->ofile.name);
}

void print_meta_single(t_file *file, t_mach_o *m)
{
	(void)file;
	(void)m;
}

void print_machs(t_file *file, t_mach_o *m)
{
	if (!m)
	{
		(file->is_multi) ? ft_putstr("") : ft_putstr("");
        return;
	}
    else if (m)
    {
		file->print_meta(file, m);
        print_in_order(file->flags, m->symbols, m->symbols);
		free_symbols(m->symbols);
		m->symbols = NULL;
    }
    print_machs(file, m->next);
}