/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:49:45 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/03 14:53:11 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>
#include <limits.h>

static bool get_is_ppc(t_mach_o *m)
{
    // dprintf(2, "m->magic  ::  %x\n", m->magic);
    // dprintf(2, "m->cputype  :                :  %d\n", m->cputype);
    
    return ((m->cputype) == CPU_TYPE_POWERPC);
}

// static int print_text(t_mach_o *m, size_t len, void *addr, uint32_t offset)
// {
//     unsigned char	*byte_addr;
//     void            *test;
//     bool            is_ppc;
//     size_t          i;

//     is_ppc = get_is_ppc(m);
//     // dprintf(2, "is_ppc  ::  %d\n", is_ppc);
// 	byte_addr = (unsigned char *)m->data + offset;
//     test = (void *)byte_addr;
// 	i = 0;
// 	while (i < len)
// 	{
// 		if ((i % 16) == 0)
// 		{
// 			if (i != 0)
// 				ft_putendl("");
// 			if (m->m64)
// 				ft_printf("%016llx", (uint64_t)&(addr[i]));
// 			else
// 				ft_printf("%08llx", (uint32_t)&(addr[i]));
// 			ft_putstr("\t");
// 		}
// 		ft_printf("%02hhx ", byte_addr[i]);
// 		i++;
//     }
//     ft_putendl("");
//     return (SUCCESS);
// }

static void print_meta_text(void)
{
    ft_printf("Contents of (%s,%s) section\n", SEG_TEXT, SECT_TEXT);
}

static void print_otool_meta_fat(t_file *file, t_mach_o *m)
{
    ft_printf("%s", file->filename);
    // dprintf(2, "get_is_ppc(m)  ::  %d\n", get_is_ppc(m));
    if (!get_is_ppc(m))
        ft_printf(" (architecture %s)", m->arch.arch_info.name);
    ft_printf(":\n");
    print_meta_text();
}

static void print_otool_meta_single(t_file *file, t_mach_o *m)
{
    (void)m;
    ft_printf("%s:\n", file->filename);
    print_meta_text();
}

static int get_otool_line(t_mach_o *m, uint64_t size, void *start, void *addr)
{
    size_t      i;
    size_t      j;
    (void)start;

    i = 0;
    j = 0;
    while (i < size)
    {
        if (m->m64)
            ft_printf("%016llx", (uint64_t)&(addr[i]));
        else
            ft_printf("%08llx", (uint32_t)&(addr[i]));
        ft_putendl("");
        i += 16;
        // j++;
    }
    return (SUCCESS);
}

static int print_otool(t_file *file, t_mach_o *m)
{
    uint64_t size;
    uint32_t offset;
    void *addr;
    void *start;

    if (!m || !(m->nsects & TEXT_SECT))
        return EXIT_SUCCESS;
    if (!file || !file->mach)
        return EXIT_FAILURE;

    if (file->info & IS_FAT)
        print_otool_meta_fat(file, m);
    else
        print_otool_meta_single(file, m);
    addr = (void *)(m->m64 ?
                m->swap64((*(t_section_64 *)(m->text_sect)).addr)
                : m->swap32((*(t_section *)(m->text_sect)).addr));
    size = (uint64_t)(m->m64 ?
                m->swap64((*(t_section_64 *)(m->text_sect)).size)
                : (uint64_t)(m->swap32((*(t_section *)(m->text_sect)).size)));
    offset = (uint64_t)m->swap32(m->m64 ? (*(t_section_64 *)(m->text_sect)).offset : (*(t_section *)(m->text_sect)).offset);
    start = (void *)m->data + offset;
    get_otool_line(m, size, start, addr);
    // print_text(m, size, addr, offset);
    print_otool(file, m->next);
    return (SUCCESS);
}

void add_file_otool(void *data, off_t size, char *argname)
{
	t_file *file;

    if (!(file = (t_file *)malloc(sizeof(t_file))))
    {
        error_ot(argname, 1, "allocation error");
        return;
    }
	if (init_file(file, data, size, argname) > SUCCESS)
    {
        free_file(file);
        error(argname, 1);
		return;
    }
	if (process_file(file, size) == SUCCESS)
        print_otool(file, file->mach);
    else
        error_ot(argname, 1, NULL);
    free_file(file);
}

void read_file_otool(t_context *c, char *av)
{
    int     fd;
    struct  stat buf;
    void    *ptr;

    // if (c->nfiles > 1)
    //     ft_printf("\n%s:\n", av);
    if ((fd = open(av, O_RDONLY)) < 0)
        c->err = (error_ot(av, 1, NULL));
    if (!c->err && fstat(fd, &buf) < 0)
        c->err = (error_ot(av, 1, NULL));
    if (!c->err && (ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        c->err = (error_ot(av, 1, NULL));
    if (!c->err)
        add_file_otool(ptr, buf.st_size, av);
    return;
}

int check_for_flags(int argc, char *argv[], t_context *c)
{
    while (--argc > 0)
    {
        if (argv[argc][0] == '-')
        {
            if (ft_strcmp(argv[argc], "-t"))
                return(error_ot(argv[argc], ERR_USAGE, NULL));
        } 
        else
            c->nfiles++;
    }
    return (EXIT_SUCCESS);
}

t_context init_context(void)
{
    t_context c;
    c.is_nm = FALSE;
    c.flags = 0x00000000;
    c.err = 0;
    c.nfiles = 0;
    return c;
}

int main(int argc, char *argv[])
{
    int             i;
    t_context       c;

    c = init_context();
    i = 0;
    if ((c.err = check_for_flags(argc, argv, &c)) > EXIT_SUCCESS)
        return (c.err);
    while (++i < argc)
    {
        if (argv[i][0] != '-')
            read_file_otool(&c, argv[i]);
    }
    if (!c.nfiles)
        error_ot(NULL, 3, "No file.");
    return (0);
}