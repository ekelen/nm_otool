/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   section.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 11:17:51 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 11:24:20 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void	update_file_section_indexes(t_m *m, const void *ptr, size_t i)
{
	char	*segname;
	char	*sectname;

	segname = m->m64 ? (*(struct section_64 *)ptr).segname
		: (*(struct section *)ptr).segname;
	sectname = m->m64 ? (*(struct section_64 *)ptr).sectname
		: (*(struct section *)ptr).sectname;
	if (!ft_strncmp(segname, SEG_TEXT, 16) && \
		!ft_strncmp(sectname, SECT_TEXT, 16) && \
		((m->text_sect = (void *)ptr)))
		m->nsects |= i;
	else if (!ft_strncmp(segname, SEG_DATA, 16) && \
		!ft_strncmp(sectname, SECT_DATA, 16))
		m->nsects |= (uint64_t)(i << 8);
	else if (!ft_strncmp(segname, SEG_DATA, 16) && \
		!ft_strncmp(sectname, SECT_BSS, 16))
		m->nsects |= (uint64_t)(i << 16);
}

int			get_secs(t_file *file, t_m *m, void *seg, uint32_t nsects)
{
	uint32_t	curr_nsects;
	void		*ptr;

	curr_nsects = m->current_sect + nsects;
	ptr = m->m64
		? ptr_check_msg((void *)file->end, seg + CMD_64_SIZE, \
			SEC_64_SIZE, "sections 64")
		: ptr_check_msg((void *)file->end, seg + CMD_SIZE, \
			SEC_SIZE, "sections");
	if (!ptr)
		return (ERR_FILE);
	while (m->current_sect < curr_nsects)
	{
		if (ptr == NULL)
			return (ERR_FILE);
		update_file_section_indexes(m, ptr, (size_t)m->current_sect);
		ptr = m->m64
			? ptr_check_msg(file->end, ptr + SEC_64_SIZE, \
				SEC_64_SIZE, "next section 64")
			: ptr_check_msg(file->end, ptr + SEC_SIZE, \
				SEC_SIZE, "next section 32");
		m->current_sect++;
	}
	return (SUCCESS);
}
