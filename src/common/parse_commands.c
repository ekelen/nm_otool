/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 14:33:42 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/07 14:49:23 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static int		parse_seg(t_file *file, t_m *m, const struct load_command *lc)
{
	t_segment_command 		*seg;
	t_segment_command_64 	*seg64;
	uint32_t 				nsects;
	int 					alignment;

	alignment = m->m64 ? 8 : 4;
	
	if (m->swap32(lc->cmdsize) % alignment != 0)
		return (ERR_FILE);

	seg = !m->m64 ? (t_segment_command *)ptr_check_msg(m->end, (void *)lc, m->swap32(lc->cmdsize), "segment") : NULL;
	seg64 = m->m64 ? (t_segment_command_64 *)ptr_check_msg(m->end, (void *)lc, m->swap32(lc->cmdsize), "segment 64") : NULL;
	if (!seg && !seg64)
		return (ERR_FILE);
	nsects = m->m64 ? m->swap32(seg64->nsects) : m->swap32(seg->nsects);
	if (nsects > 0)
		return (get_secs(file, m, m->m64 ? (void*)seg64 : (void*)seg, nsects));
	return (SUCCESS);
}

static int	parse_mh(t_file *file, t_m *m, uint32_t *ncmds, struct load_command **lc)
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

int			get_segments(t_file *file, t_m *m)
{
	struct load_command		*lc;
	uint32_t				ncmds;
	size_t					cmdsize;

	ncmds = 0;
	lc = NULL;
	if (parse_mh(file, m, &ncmds, &lc) > SUCCESS)
		return (ERR_FILE);
	ncmds++;
	while (--ncmds > 0)
	{
		cmdsize = lc ? m->swap32(lc->cmdsize) : 0;
		if (!lc || !(lc = (t_load_command *)ptr_check_msg(m->end_commands, \
			lc, cmdsize, "load command")))
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

