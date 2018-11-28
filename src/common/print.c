#include <nm_otool.h>
#include <assert.h>

void print_symbol(t_symbol *s)
{
	if ((s->print & 1) & SHOW_VALUE)
		ft_printf(s->m64 ? "%016llx " : "%08llx ", s->n_value);
	else
		ft_printf(s->m64 ? "%016c " : "%08c ", ' ');
	return;
}

void print_in_order(t_symbol *head, t_symbol *current)
{
    if (!current)
        return;
    print_in_order(head, current->left);
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
    print_in_order(head, current->right);
}

void print_nsecs(t_mach_o *m)
{
	// (void)m;
    ft_printf("TEXT  ::  %zu\n", (m->nsects & TEXT_SECT));
    ft_printf("BSS   ::  %zu\n", (m->nsects & BSS_SECT) >> 16);
    ft_printf("DATA  ::  %zu\n", (m->nsects & DATA_SECT) >> 8);
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

static int free_symbols(t_symbol *curr)
{
    if (!curr)
        return EXIT_SUCCESS;
    free_symbols(curr->left);
    free_symbols(curr->right);
    free((void*)curr);
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
        print_in_order(m->symbols, m->symbols);
		free_symbols(m->symbols);
		m->symbols = NULL;
    }
    print_machs(file, m->next);
}