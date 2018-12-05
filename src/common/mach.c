#include <nm_otool.h>
#include <assert.h>

static int	parse_mh(t_file *file, t_mach_o *m, uint32_t *ncmds, struct load_command **lc)
{
	t_u_mh mh;
	size_t sizeofcmds;

	if (m->m64)
		mh.mh64 = (t_mach_header_64 *)ptr_check(file->end, (void *)m->data, m->offset);
	else
		mh.mh32 = (t_mach_header *)ptr_check(file->end, (void *)m->data, m->offset);
	if (!mh.mh64 && !mh.mh32)
		return (ERR_FILE);
	m->cputype = m->swap32(m->m64 ? mh.mh64->cputype : mh.mh32->cputype);
	sizeofcmds = m->swap32(m->m64 ? mh.mh64->sizeofcmds : mh.mh32->sizeofcmds);
	if (!(ptr_check_msg(file->end, (void *)m->data + m->offset, sizeofcmds, "all load commands")))
		return (ERR_FILE);
	*ncmds = m->swap32(m->m64 ? mh.mh64->ncmds : mh.mh32->ncmds);
	m->end_commands = m->data + m->offset + sizeofcmds;
	if (!(*lc = (t_load_command *)ptr_check_msg(m->end_commands, (void *)m->data + m->offset, sizeof(t_load_command), "first load command")))
		return (ERR_FILE);
	return (SUCCESS);
}

// Loops thru segments; collects everything we'll need to display
static int    fill_mach(t_file *file, t_mach_o *m)
{
	struct load_command 	*lc;
	uint32_t 				ncmds;
	size_t 					cmdsize;

	ncmds = 0;
	lc = NULL;
	if (parse_mh(file, m, &ncmds, &lc) > SUCCESS)
		return (ERR_FILE);
	ncmds++;
    while (--ncmds > 0)
    {
		cmdsize = lc ? m->swap32(lc->cmdsize) : 0;
        if (lc == NULL || \
			(lc = (t_load_command *)ptr_check_msg(m->end_commands, lc, cmdsize, "load command")) == NULL)
				return (ERR_FILE);
        if (m->swap32(lc->cmd) == LC_SEGMENT || m->swap32(lc->cmd) == LC_SEGMENT_64)
        {
            if (parse_seg(file, m, lc) > SUCCESS)
                return (ERR_FILE);
        }
        else if ((m->swap32(lc->cmd) == LC_SYMTAB) && (parse_symtab(file, m, lc)) > SUCCESS)
			return (ERR_FILE);
		lc = (t_load_command *)ptr_check(m->end_commands, (void *)lc + cmdsize, sizeof(t_load_command));
    }
    return (SUCCESS);
}

static int parse_magic(uint32_t magic, t_mach_o *m)
{
	m->magic = magic;
	m->swap = (m->magic & CIGAM_MASK) == SWAP_MAGIC ? 1 : 0;
	m->swap32 = m->swap ? swap32 : nswap32;
	m->swap64 = m->swap ? swap64 : nswap64;
	if ((m->swap32(m->magic) & MAGIC_MASK) != MH_ANY)
		return (ERR_FILE);
	m->m64 = m->swap32(m->magic) & 1;
	m->offset = m->m64 ? sizeof(t_mach_header_64) : sizeof(t_mach_header);
	return (SUCCESS);
}

int		init_mach_o(t_file *file, const void *data, size_t size, t_mach_o *m)
{
	void 		*mptr;


	m->symbols = NULL;
	m->st = NULL;
	m->next = NULL;
	m->data = data;
	m->nsects = 0x00000000;
	m->current_sect = 1;
	if (!(m->end = ptr_check_msg(file->end, (void *)data + size, 0, "mach-o end")))
		return (ERR_FILE);
	if (!(mptr = ptr_check_msg(m->end, (void *)data, sizeof(uint32_t), "magic number")))
		return (ERR_FILE);
	if (parse_magic(*(uint32_t *)mptr, m) > SUCCESS)
		return (ERR_FILE);
	if (fill_mach(file, m) > SUCCESS)
		return (ERR_FILE);
	get_meta_print(file, m);
	return (SUCCESS);
}

int add_mach(t_mach_o **curr, t_mach_o *new)
{
	if (!new)
		return (ERR_FILE);
	if (!*curr)
	{
		*curr = new;
		return (SUCCESS);
	}
    return add_mach(&(*curr)->next, new);
}

void remove_mach(t_mach_o *m)
{
	if (m->symbols != NULL)
		free_symbols(m->symbols);
	free(m);
	return;
}

void free_machs(t_mach_o *curr)
{
    t_mach_o *tmp;
	while (curr)
	{
		if (curr->symbols != NULL)
		{
			free_symbols(curr->symbols);
			curr->symbols = NULL;
		}
		tmp = curr;
		curr = curr->next;
		free(tmp);
	}
	return;
}
