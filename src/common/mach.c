#include <nm_otool.h>
#include <assert.h>

// Loops thru segments; collects everything we'll need to display
// TODO: Eyesore
static int    fill_mach(t_file *file, t_mach_o *m)
{
	struct load_command *lc;
	t_mach_header_64 *mh64;
	t_mach_header *mh;
	size_t i;
	size_t ncmds;
	size_t sizeofcmds;

	void *endcmds;

	i = -1;
    mh64 = m->m64 ? (t_mach_header_64 *)ptr_check(file->end, m->data, m->offset) : NULL;
	mh = !m->m64 ? (t_mach_header *)ptr_check(file->end, m->data, m->offset) : NULL;

	m->cputype = m->m64 ? m->swap32(mh64->cputype) : m->swap32(mh->cputype);
	if (!mh64 && !mh)
		return EXIT_FAILURE;
	sizeofcmds = m->m64 ? m->swap32(mh64->sizeofcmds) : m->swap32(mh->sizeofcmds);
    lc = (t_load_command *)ptr_check_msg(file->end, m->data + m->offset, sizeofcmds, "all load commands");
	if (!lc)
		return EXIT_FAILURE;

	ncmds = !m->m64 ? m->swap32(mh->ncmds) : m->swap32(mh64->ncmds);
	
	endcmds = (void *)lc + sizeofcmds;
	// dprintf(2, "endcmds  ::  %p\n", endcmds);

    while (++i < ncmds)
    {
        lc = (t_load_command *)ptr_check_msg(endcmds, (void *)lc, m->swap32(lc->cmdsize), "load command");
        if (lc == NULL)
            return EXIT_FAILURE;
        if (m->swap32(lc->cmd) == LC_SEGMENT || m->swap32(lc->cmd) == LC_SEGMENT_64)
        {
            if (parse_seg(file, m, lc) == EXIT_FAILURE)
                return EXIT_FAILURE;
        }
        else if (m->swap32(lc->cmd) == LC_SYMTAB) {
			int result;
			result = parse_symtab(file, m, lc);
			if (result == EXIT_FAILURE)
				return EXIT_FAILURE;
        }
		if (i < ncmds - 1)
		{
        	lc = (t_load_command *)ptr_check_msg(endcmds, (void *)lc + m->swap32(lc->cmdsize), sizeof(t_load_command), "next load command");
        	if (lc == NULL && i < (ncmds - 1))
			{
        	    return EXIT_FAILURE;
			}
		}
    }
    return EXIT_SUCCESS;
}

// TODO: Get rid of redundant info, split into more helpful subfns
t_mach_o *init_mach_o(t_file *file, void *data, size_t size)
{
	t_mach_o *m;
	uint32_t magic;

	if (!(m = (t_mach_o *)malloc(sizeof(t_mach_o))))
		return NULL;

	m->data = data;

	if (!(m->end = ptr_check_msg(file->end, data + size, 0, "mach-o end")))
		return NULL;
	if (!(magic = *(uint32_t *)ptr_read(data, size, data, sizeof(uint32_t))))
		return (NULL);
	// if (!(magic = *(uint32_t *)ptr_check_msg(m->end, data, sizeof(uint32_t), "magic number")))
	// 	return (NULL);
	m->swap = (magic & CIGAM_MASK) == SWAP_MAGIC ? 1 : 0;
	m->flags = file->flags;
	
	m->swap32 = m->swap ? swap32 : nswap32;
	m->swap64 = m->swap ? swap64 : nswap64;
	// magic = m->swap32(magic);

	if ((m->swap32(magic) & MAGIC_MASK) != MH_ANY)
		return NULL;
	m->magic = magic;
	m->m64 = m->swap32(m->magic) & 1;
	m->symbols = NULL;
	m->st = NULL;
	m->offset = m->m64 ? sizeof(t_mach_header_64) : sizeof(t_mach_header);
	m->nsects = 0x00000000;
	m->current_sect = 1;
	m->text_sect = NULL;
	m->is_multi = FALSE;
	m->print_meta = NULL;
	m->next = NULL;
	if (fill_mach(file, m) == EXIT_FAILURE)
		return NULL;
	return m;
}

int add_mach(t_mach_o **curr, t_mach_o *new)
{
	if (!new)
		return EXIT_FAILURE;
	if (!*curr)
	{
		*curr = new;
		return EXIT_SUCCESS;
	}
    return add_mach(&(*curr)->next, new);
}
