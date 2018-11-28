/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:49:45 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/27 21:26:37 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static bool get_is_ppc(t_mach_o *m)
{
    dprintf(2, "m->magic  ::  %x\n", m->magic);
    dprintf(2, "m->cputype  :                :  %d\n", m->cputype);
    
    return ((m->cputype) == CPU_TYPE_POWERPC);
}

static int print_text(t_mach_o *m, int len, void *addr, uint32_t offset)
{
    unsigned char	*byte_addr;
    bool            is_ppc;
    size_t          i;

    is_ppc = get_is_ppc(m);
    dprintf(2, "is_ppc  ::  %d\n", is_ppc);
	byte_addr = m->data + offset;
	i = 0;
	while (i < len)
	{
		if ((i % 16) == 0)
		{
			if (i != 0)
				ft_putendl("");
			if (m->m64)
				ft_printf("%016llx", (uint64_t) & (addr[i]));
			else
				ft_printf("%08llx", (uint32_t) & (addr[i]));
			ft_putstr("\t");
		}
		ft_printf("%02x", byte_addr[i]);
        if (!is_ppc)
		    ft_putstr(" ");
        else
            // if (((i + 1) % 4) == 0 && !((i + 1) % 16 == 0))
            if (((i + 1) % 4) == 0)
                ft_putstr(" ");
		i++;
    }
    ft_putendl("");
}

static void print_meta_text(void)
{
    ft_printf("Contents of (%s,%s) section\n", SEG_TEXT, SECT_TEXT);
}

static void print_otool_meta_fat(t_file *file, t_mach_o *m)
{
    ft_printf("%s", file->filename);
    dprintf(2, "get_is_ppc(m)  ::  %d\n", get_is_ppc(m));
    if (!get_is_ppc(m))
        ft_printf(" (architecture %s)", m->arch.arch_info.name);
    ft_printf(":\n");
    print_meta_text();
}

static void print_otool_meta_single(t_file *file, t_mach_o *m)
{
    ft_printf("%s:\n", file->filename);
    print_meta_text();
}

static int print_otool(t_file *file, t_mach_o *m)
{
    uint64_t size;
    uint32_t offset;
    void *addr;

    if (!m || !m->nsects & TEXT_SECT)
        return EXIT_SUCCESS;
    if (!file || !file->mach)
        return EXIT_FAILURE;

    if (file->is_fat)
        print_otool_meta_fat(file, m);
    else
        print_otool_meta_single(file, m);
    addr = (void *)(m->m64 ?
                m->swap64(m->text_sect->sc.sc64->addr)
                : m->swap32(m->text_sect->sc.sc->addr));
    size = (uint64_t)(m->m64 ?
                m->swap64(m->text_sect->sc.sc64->size)
                : m->swap32(m->text_sect->sc.sc->size));
    offset = (uint64_t)m->m64 ? 
        m->swap32(m->text_sect->sc.sc64->offset)
        : m->swap32(m->text_sect->sc.sc->offset);
    print_text(m, size, addr, offset);
    print_otool(file, m->next);
}

static int start_otool(void *ptr, size_t size, char *argname)
{
    int result;
    t_file *file;

    if (!(file = init_file((void *)ptr, size, argname, NULL)))
        return EXIT_FAILURE;

    if (file->is_fat)
        result = handle_fat(file);
    else if (file->is_statlib)
        result = handle_archive(file);
    else if (!file->is_statlib && !file->is_fat)
        result = (((file->mach = init_mach_o(file, file->data, size))) == NULL 
            ? EXIT_FAILURE 
            : EXIT_SUCCESS);

    if (result == EXIT_FAILURE)
        return (error_extended(argname, 1, "Input file error."));
    else
        return print_otool(file, file->mach);
}

static int read_file(char *av)
{
    int fd;
    struct stat buf;
    void *ptr;
    int error;

    if ((fd = open(av, O_RDONLY)) < 0)
        return (error_extended(av, 1, "Couldn't open file"));
    if (fstat(fd, &buf) < 0)
        return (error_extended(av, 1, "fstat failure"));
    if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        return (error_extended(av, 1, "Couldn't allocate space with munmap"));

    return(start_otool(ptr, buf.st_size, av));
}

int main(int argc, char *argv[])
{
    int err;
    size_t i;
    size_t nfiles;
    
    //TODO: a.out if no file
    //TODO: multiple files
    err = 0;
    i = argc;
    nfiles = 0;
    
    if (argc < 1)
        return (error("No file", 3));

    while (--i > 0)
    {
        if (argv[argc - i][0] == '-')
            if (ft_strcmp(argv[argc - i], "-t"))
                return(error(argv[argc - i], 2));
    }

    i = argc;
    while (--i > 0)
    {
        if (argv[argc - i][0] != '-' && (++nfiles))
            read_file(argv[argc - i]);
    }
    if (!nfiles)
        return (error("No file", 3));
    
    return (0);
}