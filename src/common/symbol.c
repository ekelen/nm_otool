#include <nm_otool.h>
#include <assert.h>

void free_symbols(t_sym *curr)
{
    if (!curr)
        return;
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
    return '?';
}

char parse_type(uint64_t nsects, t_sym *symbol)
{
	static int i = 0;
    uint8_t c;
    c = symbol->n_type;

    if (c & N_STAB)
        return '-';
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

static int cmp_nom(t_sym *sym1, t_sym *sym2)
{
    return(ft_strcmp(sym1->nom, sym2->nom));
}

static int cmp_type(t_sym *sym1, t_sym *sym2)
{
    if (ft_strchr("Uu", sym2->type) && ft_strchr("Uu", sym1->type))
        return(cmp_nom(sym1, sym2));
    return (ft_strchr("Uu", sym2->type) - ft_strchr("Uu", sym1->type));
}

int64_t cmp_value(t_sym *sym1, t_sym *sym2, bool r)
{
    int64_t     cmp;
    int         cmp_mod;

    cmp_mod = r ? -1 : 1;

    cmp = ((int64_t)sym1->n_value - (int64_t)sym2->n_value);
    if (cmp != 0)
        return (cmp * cmp_mod);
    cmp = (int64_t)cmp_type(sym1, sym2);
    return (cmp * cmp_mod);
}

int64_t cmp_name(t_sym *sym1, t_sym *sym2, bool r)
{
    int64_t     cmp;
    int         cmp_mod;

    cmp_mod = r ? -1 : 1;
    cmp = (int64_t)cmp_nom(sym1, sym2);
    if (cmp != 0)
        return (cmp * cmp_mod);
    cmp = ((int64_t)sym1->n_value - (int64_t)(sym2->n_value));
    return (cmp * cmp_mod);
}

static char *get_sym_name(t_m *m, t_symtab_command *st, t_u_nl nl)
{
    char        *name;
    void        *name_start;
    void        *strtab_end;
    uint32_t    n_strx;

	n_strx = m->m64 ? m->swap32(nl.nl64->n_un.n_strx) : m->swap32(nl.nl32->n_un.n_strx);
	
    name_start = (void *)m->data 
        + st->stroff
        + n_strx;

    strtab_end = (void *)m->data
        + st->stroff
        + st->strsize;

    if (!(name = (char *)ptr_check_msg(strtab_end, (void *)name_start, 0, "symbol name")))
		return (NULL);
	return name;
}

static void sort_symbol(int64_t (*sort)(t_sym *s1, t_sym *s2, bool r), t_sym **curr, t_sym *new, bool r)
{
    if (!sort)
        return;
    if (!*curr)
    {
        *curr = new;
        return;
    }
    if (sort((*curr), new, r) > 0)
        return sort_symbol(sort, &(*curr)->left, new, r);
    else
        return sort_symbol(sort, &(*curr)->right, new, r);
}

static int fill_symbol_data(t_m *m, t_symtab_command *st, t_sym *s)
{
	t_u_nl nl;

	s->m64 = m->m64;
	if (s->m64)
		nl.nl64 = (t_nlist_64 *)(s->nptr);
	else
		nl.nl32 = (t_nlist *)(s->nptr);
	if (!(s->nom = get_sym_name(m, st, nl)))
        return (ERR_FILE);
	s->n_value = s->m64 ? m->swap64(nl.nl64->n_value) : (uint64_t)(m->swap32(nl.nl32->n_value));
	s->n_sect = s->m64 ? nl.nl64->n_sect : nl.nl32->n_sect;
	s->n_type = s->m64 ? nl.nl64->n_type : nl.nl32->n_type;
	s->type = parse_type(m->nsects, s);
	return (SUCCESS);
}

int add_symbol(t_file *file, t_m *m, t_symtab_command *st, const void *nptr)
{
	t_sym *s;
    if (!(s = malloc(sizeof(t_sym))))
        return (EXIT_FAILURE);
	s->nptr = nptr;
	s->left = NULL;
	s->right = NULL;
	if (fill_symbol_data(m, st, s))
    {
        free(s);
		return (EXIT_FAILURE);
    }
    sort_symbol(file->sort, &(m->symbols), s, (file->flags & SORT_REVERSE));
    return (EXIT_SUCCESS);
}