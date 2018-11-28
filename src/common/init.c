/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 11:29:00 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/28 09:18:37 by ekelen           ###   ########.fr       */
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
	file->swap = magic == MH_CIGAM || magic == MH_CIGAM_64 || magic == FAT_CIGAM || magic == FAT_CIGAM_64 ? 1 : 0;
	file->swap32 = file->swap ? swap32 : nswap32;
	file->is_fat = magic == FAT_MAGIC || magic == FAT_CIGAM || magic == FAT_CIGAM_64 || magic == FAT_MAGIC_64 ? 1 : 0;
	file->is_statlib = !ft_strncmp((const char*)data, ARMAG, SARMAG);
	file->is_multi = (bool)(file->is_fat || file->is_statlib);
	file->m64 = file->swap32(magic) & 1  || file->is_statlib ? 1 : 0;
	file->offset = file_offset(file->is_fat, file->is_statlib, file->m64);
	file->print_meta = file->is_fat 
		? print_meta_fat
		: (file->is_statlib 
			? print_meta_statlib
			: print_meta_single);
	file->mach = NULL;
    return file;
}