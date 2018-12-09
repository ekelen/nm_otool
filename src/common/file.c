/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 11:29:00 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/09 11:37:09 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static size_t	file_offset(uint32_t info)
{
	if (info & IS_FAT)
		return (sizeof(struct fat_header));
	else if (info & IS_STATLIB)
		return ((size_t)SARMAG);
	else
		return ((info & IS_64) ? sizeof(struct mach_header_64)
			: sizeof(struct mach_header));
}

static uint32_t	parse_hdr_type(uint32_t magic, void *data, \
	uint32_t (*uswap32)(uint32_t x))
{
	uint32_t	type;
	int			is_fat;
	int			is_statlib;
	int			is_single_mach;

	type = 0x0000;
	is_statlib = (int)(!ft_strncmp((const char*)data, ARMAG, SARMAG));
	is_fat = (int)((uswap32(magic) & MAGIC_MASK) == FAT_ANY);
	is_single_mach = (int)(!is_fat && !is_statlib);
	type |= (is_single_mach << 2);
	type |= (is_fat << 3);
	type |= (is_statlib << 4);
	type |= ((!is_single_mach) << 5);
	return (type);
}

static uint32_t	parse_magic(uint32_t magic, void *data)
{
	uint32_t	info;
	int			is_64;
	int			is_swap;
	uint32_t	(*uswap32)(uint32_t x);

	info = 0x0000;
	is_swap = (magic & CIGAM_MASK) == SWAP_ANY;
	info |= is_swap << 1;
	uswap32 = (info & IS_SWAP) ? swap32 : nswap32;
	info |= parse_hdr_type(magic, data, uswap32);
	is_64 = (int)((uswap32(magic) & 1) || (info & IS_STATLIB));
	info |= is_64;
	return (info);
}

int				init_file(t_file *file, void *data, off_t size, char *argname)
{
	void *ptr;

	file->mach = NULL;
	if (!(file->data = ptr_check(data + size, data, size)))
		return (EXIT_FAILURE);
	file->flags = 0;
	file->length = (size_t)size;
	file->end = data + size;
	file->filename = argname;
	if (!(ptr = ptr_check(file->end, data, sizeof(uint32_t))))
		return (EXIT_FAILURE);
	file->info = parse_magic(*(uint32_t *)ptr, data);
	file->swap32 = file->info & IS_SWAP ? swap32 : nswap32;
	file->swap64 = file->info & IS_SWAP ? swap64 : nswap64;
	file->offset = file_offset(file->info);
	file->sort = NULL;
	return (SUCCESS);
}

void			free_file(t_file *file)
{
	if (file->mach)
		free_machs(file->mach);
	file->mach = NULL;
	munmap((void *)file->data, (size_t)(file->length));
	free(file);
	return ;
}
