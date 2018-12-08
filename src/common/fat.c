/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 10:23:19 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 10:30:41 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

t_arch			init_arch(t_file *file, t_u_fa f)
{
	t_arch			new;
	cpu_type_t		cputype;
	cpu_subtype_t	cpusubtype;

	new.offset = file->info & IS_64 ? FH_64_SIZE : FH_SIZE;
	cputype = file->info & IS_64 ? file->swap32(f.fa64->cputype)
		: file->swap32(f.fa32->cputype);
	cpusubtype = file->info & IS_64 ? file->swap32(f.fa64->cpusubtype)
		: file->swap32(f.fa32->cpusubtype);
	new.arch_info = get_arch_info(cputype, cpusubtype);
	new.fa_offset = file->info & IS_64 ? file->swap64(f.fa64->offset)
		: (uint64_t)(file->swap32(f.fa32->offset));
	new.size = file->info & IS_64 ? file->swap64(f.fa64->size)
		: (uint64_t)(file->swap32(f.fa32->size));
	return (new);
}

static int		add_arch(t_file *file, t_arch a, bool is_host)
{
	t_m		*m;
	int		result;

	if (!(m = (t_m *)malloc(sizeof(t_m))))
		return (ERR_ALLOCATION);
	if ((result = init_mach_o(file, (void *)file->data + a.fa_offset, \
		a.size, m)) > SUCCESS)
	{
		remove_mach(m);
		return (result);
	}
	m->arch = a;
	if (is_host)
	{
		file->info &= ~IS_MULTI;
		get_meta_print(file, m);
	}
	if ((result = add_mach(&(file->mach), m)) > SUCCESS)
		remove_mach(m);
	return (result);
}

static t_u_fa	check_fa(t_file *file, uint32_t nfat_arch, int i)
{
	t_u_fa f;

	if (file->info & IS_64)
		f.fa64 = (t_fat_arch_64 *)ptr_check_msg(file->end, \
			(void *)file->data + file->offset + FH_64_SIZE * i, \
			FH_64_SIZE * nfat_arch, "fat arch 64");
	else
		f.fa32 = (t_fat_arch *)ptr_check_msg(file->end, \
			(void *)file->data + file->offset + FH_SIZE * i, \
			FH_SIZE, "fat arch 32");
	return (f);
}

int				handle_fat_2(t_file *file, uint32_t nfat_arch)
{
	t_u_fa	f;
	size_t	i;
	t_arch	a;

	i = -1;
	while (++i < nfat_arch)
	{
		f = check_fa(file, nfat_arch, i);
		if (!f.fa32 && !f.fa64)
			return (ERR_FILE);
		a = init_arch(file, f);
		if (a.arch_info.cpu_type == HOST_CPU)
			return (add_arch(file, a, TRUE));
	}
	i = -1;
	while (++i < nfat_arch)
	{
		f = check_fa(file, nfat_arch, i);
		if (!f.fa32 && !f.fa64)
			return (EXIT_FAILURE);
		a = init_arch(file, f);
		if (add_arch(file, a, FALSE) > EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int			handle_fat(t_file *file)
{
	struct fat_header	*hdr;
	size_t				nfat_arch;
	t_status			err;

	if (!(hdr = (struct fat_header *)ptr_check_msg(file->end, \
		(void *)file->data, file->offset, "fat header")))
		return (ERR_FILE);
	nfat_arch = file->swap32(hdr->nfat_arch);
	err = handle_fat_2(file, nfat_arch);
	return (err);
}
