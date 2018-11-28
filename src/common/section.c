#include <nm_otool.h>

t_sec *find_sect(t_sec *curr, uint8_t i)
{
    if (!curr)
        return NULL;
    else if (curr->index == i)
        return curr;
    return find_sect(curr->next, i);
}

static void update_file_section_indexes(t_mach_o *m, t_sec *sec)
{
    char *segname;
    char *sectname;

    segname = m->m64 ? sec->sc.sc64->segname : sec->sc.sc->segname;
    sectname = m->m64 ? sec->sc.sc64->sectname : sec->sc.sc->sectname;

    if (!ft_strncmp(segname, SEG_TEXT, 16) && \
        !ft_strncmp(sectname, SECT_TEXT, 16) && \
        ((m->text_sect = sec)))
            m->nsects |= sec->index;
    else if (!ft_strncmp(segname, SEG_DATA, 16) && \
        !ft_strncmp(sectname, SECT_DATA, 16))
            m->nsects |= (uint64_t)(sec->index << 8);
    else if (!ft_strncmp(segname, SEG_DATA, 16) && \
        !ft_strncmp(sectname, SECT_BSS, 16))
            m->nsects |= (uint64_t)(sec->index << 16);
}

static t_sec *init_sec(t_file *file, t_mach_o *m, const void *sec_addr, size_t index)
{
    t_sec *new;
    
    if (!(new = malloc(sizeof(t_sec))))
        return NULL;
    new->next = NULL;
    new->sec = sec_addr;
    new->index = index;
    if (m->m64)
        new->sc.sc64 = (t_section_64 *)ptr_check(file->end, sec_addr, sizeof(t_section_64));
    else
        new->sc.sc = (t_section *)ptr_check(file->end, sec_addr, sizeof(t_section));
    return new;
}

static int add_sec_2(t_mach_o *m, t_sec **curr, t_sec *new, size_t index)
{
    size_t normalized_index;
    if (!*curr)
    {
        new->index = index;
        *curr = new;
        return EXIT_SUCCESS;
    }
    normalized_index = (index == MAX_SECT || index == NO_SECT ? NO_SECT : index + 1);
    add_sec_2(m, &(*curr)->next, new, normalized_index);
}

static int add_sec(t_file *file, t_mach_o *m, const void *sec_addr)
{
    t_sec *new = init_sec(file, m, sec_addr, 1);
    if (!new)
        return (EXIT_FAILURE);
    add_sec_2(m, &(m->secs), new, 1);
	update_file_section_indexes(m, new);
	return (EXIT_SUCCESS);
}

// TODO: Don't need to store all the sects?
int get_secs(t_file *file, t_mach_o *m, void *seg, uint32_t nsects)
{
    size_t index;
    void *ptr;

    ptr = m->m64 
        ? ptr_check_msg((void *)file->end, seg + sizeof(t_segment_command_64), sizeof(t_section_64), "segment command 64")
        : ptr_check_msg((void *)file->end, seg + sizeof(t_segment_command), sizeof(t_section), "segment command");
    if (!ptr)
        return (EXIT_FAILURE);
    index = -1;
    while (++index < nsects)
    {
        if (ptr == NULL)
            return (EXIT_FAILURE);
        if ((add_sec(file, m, ptr)) == EXIT_FAILURE)
			return EXIT_FAILURE;
        ptr = m->m64
            ? ptr_check((void *)file->end, ptr + sizeof(t_section_64), sizeof(t_section_64))
            : ptr_check((void *)file->end, ptr + sizeof(t_section), sizeof(t_section));
    }
	return EXIT_SUCCESS;
}