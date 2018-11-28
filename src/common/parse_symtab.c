#include <nm_otool.h>
#include <assert.h>

static int free_symbols(t_symbol *curr)
{
    if (!curr)
        return EXIT_SUCCESS;
    free_symbols(curr->left);
    free_symbols(curr->right);
    free((void*)curr);
}

static int handle_symtab_2(t_file *file, t_mach_o *m, t_symtab_command *st)
{
    uint32_t i;
    t_nlist_64 *nlists;
    void *ptr;

    ptr = m->m64 
        ? ptr_check_msg(m->end, m->data + st->symoff, sizeof(t_nlist_64) * st->nsyms, "nlists 64")
        : ptr_check_msg(m->end, m->data + st->symoff, sizeof(t_nlist) * st->nsyms, "nlists");
    i = -1;
    while (++i < st->nsyms)
    {
        if (i > 0)
            ptr = m->m64
                ? ptr_check_msg(m->end, ptr + sizeof(t_nlist_64), sizeof(t_nlist_64), "next nlist 64")
                : ptr_check_msg(m->end, ptr + sizeof(t_nlist), sizeof(t_nlist), "next nlist");
        if (!ptr)
            return (EXIT_FAILURE);
        if (add_symbol(file, m, st, ptr) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int parse_symtab(t_file *file, t_mach_o *m, const struct load_command *cmd)
{
    t_symtab_command *st;
    void *strtab;
    int result;

    if (!(st = (t_symtab_command *)ptr_check_msg(m->end, (void *)cmd, sizeof(t_symtab_command), "symtab command")))
        return EXIT_FAILURE;
    st->strsize = m->swap32(st->strsize);
    st->stroff = m->swap32(st->stroff);
    st->symoff = m->swap32(st->symoff);
    st->nsyms = m->swap32(st->nsyms);

    if (!(strtab = ptr_check_msg(m->end, (void *)m->data + st->stroff, st->strsize, "strtab")))
        return EXIT_FAILURE;
    return (handle_symtab_2(file, m, st));
    // return (m->m64 ? handle_symtab_64(file, m, st) : handle_symtab_32(file, m, st));
}