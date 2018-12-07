/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 12:36:29 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/07 12:36:43 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void		print_otool_section(void)
{
	ft_printf("Contents of (%s,%s) section\n", SEG_TEXT, SECT_TEXT);
}

static void		print_mach_meta_fat_joined(t_file *file, t_m *m)
{
	if (file->flags & PRINT_PATHNAME)
		return ;
	if (file->info & IS_NM)
		ft_putendl("");
	ft_printf("%s", file->filename);
	if (file->info & IS_NM)
		ft_printf(" (for architecture %s)", m->arch.arch_info.name);
	else if (m->arch.arch_info.name[0])
		ft_printf(" (architecture %s)", m->arch.arch_info.name);
	ft_printf(":\n");
	if (!(file->info & IS_NM))
		print_otool_section();
	return ;
}

static void		print_meta_statlib(t_file *file, t_m *m)
{
	if ((file->info & IS_NM) && !(file->flags & PRINT_PATHNAME))
		ft_putendl("");
	else if (!(file->info & IS_NM))
		if ((void*)file->mach == (void *)m)
			ft_printf("Archive : %s\n", file->filename);
	if (!(file->flags & PRINT_PATHNAME))
		ft_printf("%s(%s):\n", file->filename, m->ofile.name);
	if (!(file->info & IS_NM))
		print_otool_section();
	return ;
}

static void		print_meta_32_64(t_file *file, t_m *m)
{
	uint32_t show_av;

	show_av = (IS_SINGLE_MACH | IS_MULTI_AV | IS_NM);
	if ((file->info & show_av) == show_av
		&& !(file->flags & PRINT_PATHNAME) \
		&& (void*)(file->mach) == (void*)m)
		ft_printf("\n%s:\n", file->filename);
	if (!(file->info & IS_NM) && (file))
	{
		ft_printf("%s:\n", file->filename);
		print_otool_section();
	}
	return ;
}

void			get_meta_print(t_file *file, t_m *m)
{
	if ((file->info & (IS_FAT | IS_MULTI)) == (IS_FAT | IS_MULTI))
		m->print_meta = print_mach_meta_fat_joined;
	else if (file->info & IS_STATLIB)
		m->print_meta = print_meta_statlib;
	else
		m->print_meta = print_meta_32_64;
	return ;
}
