#include <nm_otool.h>
#include <assert.h>

char parse_section_type(uint64_t nsects, uint8_t n_sect)
{
    t_sec *sec;
    sec = NULL;

    if (n_sect == NO_SECT)
        return 'u';
    else if (n_sect <= MAX_SECT)
    {
		if (n_sect == (nsects & TEXT_SECT))
			return 't';
		else if (n_sect == (nsects & DATA_SECT) >> 8)
			return 'd';
		else if (n_sect == (nsects & BSS_SECT) >> 16)
			return 'b';
		else
			return 's';
    }
    return '?'; // shouldn't happen
}

char parse_type(uint64_t nsects, t_symbol *symbol)
{
	static int i = 0;
    uint8_t c;
    c = symbol->n_type;

    if (c & N_STAB)
        return '-';
	// ft_printf("\n\ni  ::  %d\n", i);
	// ft_printf("symbol->n_type  ::  %x\n", symbol->n_type);
    c = c & N_TYPE;
	// ft_printf("symbol->n_type  ::  %x\n", symbol->n_type);
	// ft_printf("c  ::  %d\n", c);
	// ft_printf("N_ABS  ::  %d\n", N_ABS);
    if (c == N_UNDF)
        c = symbol->n_value == 0 ? 'u' : 'c'; // local common symbol
    else if (c == N_PBUD)
        c = 'u';
    else if (c == N_ABS)
	{
        c = 'a';
	}
    else if (c == N_SECT)
        c = parse_section_type(nsects, symbol->n_sect);
    else if (c == N_INDR)
        c = 'i';
    else
        c = 'z';
    if (symbol->n_type & N_EXT)
        c -= 32;
	i = i + 1;
    return (unsigned char)c;
}

static uint32_t check_show(uint32_t flags, t_symbol *symbol)
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

int cmp_name(t_symbol *sym1, t_symbol *sym2)
{
    int cmp;
    
    cmp = ft_strcmp(sym1->nom, sym2->nom);
    if (cmp == 0)
        return (sym1->n_value - sym2->n_value);
    else
        return cmp;
}

static char *get_sym_name(t_mach_o *m, t_symtab_command *st, t_u_nl nl)
{
    char *name;
    void *name_start;
    void *strtab_end;
    uint32_t n_strx;

	n_strx = m->m64 ? m->swap32(nl.nl64->n_un.n_strx) : m->swap32(nl.nl32->n_un.n_strx);
	
    name_start = m->data 
        + st->stroff
        + n_strx;

    strtab_end = m->data
        + st->stroff
        + st->strsize;

    if (!(name = (char *)ptr_check_msg(strtab_end, name_start, 0, "symbol name")))
		return NULL;
	return name;
}

void sort_symbol(t_symbol **curr, t_symbol *new)
{
    int cmp;
    
    if (!*curr)
    {
        *curr = new;
        return;
    }
    cmp = (*curr)->sort((*curr), new);
    if (cmp > 0)
        return sort_symbol(&(*curr)->left, new);
    else
        return sort_symbol(&(*curr)->right, new);
}

static int fill_symbol_data(uint32_t flags, t_mach_o *m, t_symtab_command *st, t_symbol *s)
{
	t_u_nl nl;
	s->m64 = m->m64;

	if (s->m64)
		nl.nl64 = (t_nlist_64 *)(s->nptr);
	else
		nl.nl32 = (t_nlist *)(s->nptr);

	if (!(s->nom = get_sym_name(m, st, nl)))
        return EXIT_FAILURE;
	// ft_printf("s->m64  ::  %d\n", s->m64);
	s->n_value = s->m64 ? m->swap64(nl.nl64->n_value) : (uint64_t)(m->swap32(nl.nl32->n_value));
	s->n_sect = s->m64 ? nl.nl64->n_sect : nl.nl32->n_sect;
	s->n_type = s->m64 ? nl.nl64->n_type : nl.nl32->n_type;
	
	s->type = parse_type(m->nsects, s);
    s->sort = cmp_name;

	s->print = check_show(flags, s);
	return EXIT_SUCCESS;
}

int add_symbol(t_file *file, t_mach_o *m, t_symtab_command *st, const void *nptr)
{
	t_symbol *s;
    if (!(s = malloc(sizeof(t_symbol))))
        return EXIT_FAILURE;
	s->nptr = nptr;
	s->print = 0x00000000;
	s->left = NULL;
	s->right = NULL;
	
	if (fill_symbol_data(file->flags, m, st, s) == EXIT_FAILURE)
		return EXIT_FAILURE; // free
    sort_symbol(&(m->symbols), s);
    return EXIT_SUCCESS;
}