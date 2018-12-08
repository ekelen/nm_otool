/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 14:33:42 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 12:56:20 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static int		parse_seg(t_file *file, t_m *m, const struct load_command *lc)
{
	uint32_t					nsects;
	int							alignment;
	struct segment_command		*seg;
	struct segment_command_64	*seg64;

	alignment = m->m64 ? 8 : 4;
	if (m->swap32(lc->cmdsize) % alignment != 0)
		return (ERR_FILE);
	seg = !m->m64
		? (struct segment_command *)ptr_check_msg(m->end, (void *)lc, \
		m->swap32(lc->cmdsize), "segment")
		: NULL;
	seg64 = m->m64
		? (struct segment_command_64 *)ptr_check_msg(m->end, (void *)lc, \
		m->swap32(lc->cmdsize), "segment 64")
		: NULL;
	if (!seg && !seg64)
		return (ERR_FILE);
	nsects = m->m64 ? m->swap32(seg64->nsects) : m->swap32(seg->nsects);
	if (nsects > 0)
		return (get_secs(file, m, m->m64 ? (void*)seg64 : (void*)seg, nsects));
	return (SUCCESS);
}

static int		parse_mh(t_file *file, t_m *m, uint32_t *ncmds, \
	struct load_command **lc)
{
	t_u_mh	mh;
	size_t	sizeofcmds;
	void	*ptr;
	void	*cmds;

	if (!(ptr = ptr_check_msg(file->end, (void *)m->data, m->offset, \
		"mach-o header")))
		return (ERR_FILE);
	if (m->m64)
		mh.mh64 = (struct mach_header_64 *)ptr;
	else
		mh.mh32 = (struct mach_header *)ptr;
	m->cputype = m->swap32(m->m64 ? mh.mh64->cputype : mh.mh32->cputype);
	sizeofcmds = m->swap32(m->m64 ? mh.mh64->sizeofcmds : mh.mh32->sizeofcmds);
	cmds = (void *)m->data + m->offset;
	if (!(ptr_check_msg(file->end, cmds, sizeofcmds, "all load commands")))
		return (ERR_FILE);
	*ncmds = m->swap32(m->m64 ? mh.mh64->ncmds : mh.mh32->ncmds);
	m->end_cmds = cmds + sizeofcmds;
	if (!(ptr = ptr_check_msg(m->end_cmds, cmds, LC_SIZE, "first load cmd")))
		return (ERR_FILE);
	*lc = (struct load_command *)ptr;
	return (SUCCESS);
}

int				get_segments(t_file *file, t_m *m)
{
	struct load_command		*lc;
	uint32_t				ncmds;
	size_t					cmdsize;

	if (parse_mh(file, m, &ncmds, &lc) > SUCCESS)
		return (ERR_FILE);
	ncmds++;
	while (--ncmds > 0)
	{
		cmdsize = lc ? m->swap32(lc->cmdsize) : 0;
		if (!lc || !(lc = (struct load_command *)ptr_check_msg(m->end_cmds, \
			lc, cmdsize, "load command")))
			return (ERR_FILE);
		if (m->swap32(lc->cmd) == LC_SEGMENT
			|| m->swap32(lc->cmd) == LC_SEGMENT_64)
		{
			if (parse_seg(file, m, lc) > SUCCESS)
				return (ERR_FILE);
		}
		else if ((m->swap32(lc->cmd) == LC_SYMTAB)
			&& (parse_symtab(file, m, lc)) > SUCCESS)
			return (ERR_FILE);
		lc = (T_LC *)ptr_check(m->end_cmds, (void *)lc + cmdsize, LC_SIZE);
	}
	return (SUCCESS);
}
