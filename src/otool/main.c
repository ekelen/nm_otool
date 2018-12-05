/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:49:45 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/05 10:07:15 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>
#include <limits.h>

static bool get_print_block(t_mach_o *m)
{
    return (!(m->cputype == CPU_TYPE_I386 || m->cputype == CPU_TYPE_X86_64));
}

static int get_otool_line(t_mach_o *m, uint64_t size, void *start, void *addr)
{
    size_t          i;
    size_t          j;
    size_t          k;
    uint32_t        four_bytes;

    i = 0;
    j = 0;
    k = 0;
    
    while (i < size)
    {
        if (m->m64)
            ft_printf("%016llx", (uint64_t)&(addr[i]));
        else
            ft_printf("%08lx", (uint32_t)&(addr[i]));
        ft_putchar('\t');
        k = 0;
        j = 0;
        if (!get_print_block(m)) // TODO: What conditions?
        {
            while (j < 16 && i + j < size)
            {
                ft_printf("%02hhx ", *((uint8_t *)(&start[j])));
                j++;
            }
        }
        else
        {
            while (k < 16 && i + k < size)
            {
                four_bytes = m->swap32((*(uint32_t *)(start + k)));
                ft_printf("%08x ", four_bytes);
                k += 4;
            }
        }
        ft_putendl("");
        start = (void *)start + 16;
        i += 16;
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
        return (EXIT_SUCCESS);
    if (!file || !file->mach)
        return (EXIT_FAILURE);

    m->print_meta(file, m);
    addr = (void *)(m->m64 ?
                m->swap64((*(t_section_64 *)(m->text_sect)).addr)
                : m->swap32((*(t_section *)(m->text_sect)).addr));
    size = (uint64_t)(m->m64 ?
                m->swap64((*(t_section_64 *)(m->text_sect)).size)
                : (uint64_t)(m->swap32((*(t_section *)(m->text_sect)).size)));
    offset = (uint64_t)(m->m64 ?
                m->swap64((*(t_section_64 *)(m->text_sect)).offset)
                : (uint64_t)(m->swap32((*(t_section *)(m->text_sect)).offset)));
    start = (void *)m->data + offset;
    get_otool_line(m, size, start, addr);
    return print_otool(file, m->next);
}

void add_file_otool(void *data, off_t size, char *argname)
{
    t_file      *file;
    int         err;

    err = 0;
    if (!(file = (t_file *)malloc(sizeof(t_file))))
        err = ERR_ALLOCATION;
	if (!err && (err = init_file(file, data, size, argname)) > SUCCESS)
        err = ERR_FILE;
    file->info |= 0 << 6;
	if (!err && (err = process_file(file, size)) > SUCCESS)
        error_ot(argname, err, NULL);
    else
        print_otool(file, file->mach);
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
    if (!c->err && (ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
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