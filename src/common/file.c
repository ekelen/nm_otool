/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 11:29:00 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/30 11:05:46 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>
#include <assert.h>

size_t		file_offset(bool universal, bool ar, bool m64)
{
	if (universal)
		return (sizeof(struct fat_header));
	else if (ar)
		return ((size_t)SARMAG);
	else
		return (m64 ? sizeof(struct mach_header_64) : sizeof(struct mach_header));
}

t_file      *init_file(void *data, off_t size, char *argname, uint32_t flags)
{
    t_file *file;
	uint32_t magic;

    if (!(file = (t_file *)malloc(sizeof(t_file))))
		return NULL;
	file->data = data;
	file->flags = flags;
	file->length = (size_t)size;
	file->end = (void*)data + size;
	file->filename = argname;
	if (!(magic = *(uint32_t *)ptr_read(data, size, data, sizeof(uint32_t))))
		return (NULL);
	file->swap = ((magic & CIGAM_MASK) == SWAP_ANY) ? 1 : 0;
	file->swap32 = file->swap ? swap32 : nswap32;
	file->is_fat = (file->swap32(magic) & MAGIC_MASK) == FAT_ANY ? 1 : 0;
	file->is_statlib = !ft_strncmp((const char*)data, ARMAG, SARMAG);
	file->is_multi = (bool)(file->is_fat || file->is_statlib);
	file->m64 = file->swap32(magic) & 1  || file->is_statlib ? 1 : 0;
	file->offset = file_offset(file->is_fat, file->is_statlib, file->m64);
	file->sort = NULL;
	file->mach = NULL;
    return file;
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

void	free_file(t_file *file)
{
	if (file->mach)
	{
		free_machs(file->mach);
	}
	file->mach = NULL;
	munmap(file->data, (size_t)(file->length));
	free(file);
	// dprintf(2, "freed file\n");
	return;
}