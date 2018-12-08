/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 10:23:19 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/07 18:43:52 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static const t_arch_info		g_arch_list[] = {
	{"any", CPU_TYPE_ANY, CPU_SUBTYPE_MULTIPLE},
	{"arm", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_ALL},
	{"arm64", CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL},
	{"arm64", CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_V8},
	{"armv4t", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V4T},
	{"armv5", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V5TEJ},
	{"armv6", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V6},
	{"armv6m", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V6M},
	{"armv7", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7},
	{"armv7em", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7EM},
	{"armv7f", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7F},
	{"armv7k", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K},
	{"armv7m", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7M},
	{"armv7s", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7S},
	{"armv8", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V8},
	{"big", CPU_TYPE_ANY, CPU_SUBTYPE_BIG_ENDIAN},
	{"hppa", CPU_TYPE_HPPA, CPU_SUBTYPE_HPPA_ALL},
	{"hppa7100LC", CPU_TYPE_HPPA, CPU_SUBTYPE_HPPA_7100LC},
	{"i386", CPU_TYPE_I386, CPU_SUBTYPE_I386_ALL},
	{"i486", CPU_TYPE_I386, CPU_SUBTYPE_486},
	{"i486SX", CPU_TYPE_I386, CPU_SUBTYPE_486SX},
	{"i586", CPU_TYPE_I386, CPU_SUBTYPE_586},
	{"i686", CPU_TYPE_I386, CPU_SUBTYPE_PENTPRO},
	{"i860", CPU_TYPE_I860, CPU_SUBTYPE_I860_ALL},
	{"little", CPU_TYPE_ANY, CPU_SUBTYPE_LITTLE_ENDIAN},
	{"m68030", CPU_TYPE_MC680x0, CPU_SUBTYPE_MC68030_ONLY},
	{"m68040", CPU_TYPE_MC680x0, CPU_SUBTYPE_MC68040},
	{"m68k", CPU_TYPE_MC680x0, CPU_SUBTYPE_MC680x0_ALL},
	{"m88k", CPU_TYPE_MC88000, CPU_SUBTYPE_MC88000_ALL},
	{"pentIIm3", CPU_TYPE_I386, CPU_SUBTYPE_PENTII_M3},
	{"pentIIm5", CPU_TYPE_I386, CPU_SUBTYPE_PENTII_M5},
	{"pentium", CPU_TYPE_I386, CPU_SUBTYPE_PENT},
	{"pentium4", CPU_TYPE_I386, CPU_SUBTYPE_PENTIUM_4},
	{"pentpro", CPU_TYPE_I386, CPU_SUBTYPE_PENTPRO},
	{"ppc", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_ALL},
	{"ppc601", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_601},
	{"ppc603", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_603},
	{"ppc603e", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_603e},
	{"ppc603ev", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_603ev},
	{"ppc604", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_604},
	{"ppc604e", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_604e},
	{"ppc64", CPU_TYPE_POWERPC64, CPU_SUBTYPE_POWERPC_ALL},
	{"ppc7450", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_7450},
	{"ppc750", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_750},
	{"ppc970", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_970},
	{"ppc970-64", CPU_TYPE_POWERPC64, CPU_SUBTYPE_POWERPC_970},
	{"sparc", CPU_TYPE_SPARC, CPU_SUBTYPE_SPARC_ALL},
	{"x86_64", CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL},
	{"x86_64", CPU_TYPE_X86_64, 0x80000003},
	{"x86_64", CPU_TYPE_I386, CPU_SUBTYPE_X86_64_H},
	{"x86_64", CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_H},
	{"xscale", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_XSCALE},
	{"", 0, 0}
};

t_arch_info		get_arch_info(cpu_type_t cputype, cpu_subtype_t cpusubtype)
{
	int i;

	i = 0;
	while (g_arch_list[i].name[0])
	{
		if (cputype == g_arch_list[i].cpu_type
			&& cpusubtype == g_arch_list[i].cpu_subtype)
			return (g_arch_list[i]);
		i++;
	}
	return (g_arch_list[i]);
}

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
