#include <nm_otool.h>
#include <assert.h>

int free_symbols(t_symbol *curr)
{
    if (!curr)
        return EXIT_SUCCESS;
    free_symbols(curr->left);
    free_symbols(curr->right);
    free((void*)curr);
}

char parse_section_type(uint64_t nsects, uint8_t n_sect)
{
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
    c = c & N_TYPE;
    if (c == N_UNDF)
        c = symbol->n_value == 0 ? 'u' : 'c'; // local common symbol
    else if (c == N_PBUD)
        c = 'u';
    else if (c == N_ABS)
        c = 'a';
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

// default
int cmp_name(t_symbol *sym1, t_symbol *sym2)
{
    int cmp;
    
    cmp = ft_strcmp(sym1->nom, sym2->nom);
    if (cmp == 0)
        return (sym1->n_value - sym2->n_value);
    else
        return cmp;
}

// todo: test
// for flag -r
int cmp_name_reverse(t_symbol *sym1, t_symbol *sym2)
{
    int cmp;
    
    cmp = ft_strcmp(sym2->nom, sym1->nom);
    if (cmp == 0)
        return (sym2->n_value - sym1->n_value);
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

void sort_symbol(int (*sort)(t_symbol *s1, t_symbol *s2), t_symbol **curr, t_symbol *new)
{
    int cmp;
    
    if (!*curr)
    {
        *curr = new;
        return;
    }
    cmp = sort((*curr), new);
    if (cmp > 0)
        return sort_symbol(sort, &(*curr)->left, new);
    else
        return sort_symbol(sort, &(*curr)->right, new);
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
	s->n_value = s->m64 ? m->swap64(nl.nl64->n_value) : (uint64_t)(m->swap32(nl.nl32->n_value));
	s->n_sect = s->m64 ? nl.nl64->n_sect : nl.nl32->n_sect;
	s->n_type = s->m64 ? nl.nl64->n_type : nl.nl32->n_type;
	s->type = parse_type(m->nsects, s);
	return EXIT_SUCCESS;
}

int add_symbol(t_file *file, t_mach_o *m, t_symtab_command *st, const void *nptr)
{
	t_symbol *s;
    if (!(s = malloc(sizeof(t_symbol))))
        return (EXIT_FAILURE);
	s->nptr = nptr;
	s->left = NULL;
	s->right = NULL;
	if (fill_symbol_data(file->flags, m, st, s) == EXIT_FAILURE)
		return EXIT_FAILURE; // free
    sort_symbol(file->sort, &(m->symbols), s);
    return (EXIT_SUCCESS);
}