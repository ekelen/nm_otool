/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 09:14:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/06 10:09:45 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int handle_32_64(t_file *file, size_t size)
{
    if (!(file->mach = (t_mach_o *)malloc(sizeof(t_mach_o))))
		return (ERR_ALLOCATION);
    return (init_mach_o(file, file->data, size, file->mach));
}

int process_file(t_file *file, size_t size)
{
    if (file->info & IS_FAT)
        return (handle_fat(file));
    else if (file->info & IS_STATLIB)
        return(handle_archive(file));
    else if (file->info & IS_SINGLE_MACH)
        return(handle_32_64(file, size));
    else
        return(ERR_OTHER);
}

void read_file(t_context *c, char *av)
{
    int         fd;
    struct      stat buf;
    void        *ptr;

    if ((fd = open(av, O_RDONLY)) < 0)
    {
        error(av, ERR_FILE, NULL, c->is_nm);
        return;
    }
    if (fstat(fd, &buf) < 0)
    {
        error(av, ERR_FILE, NULL, c->is_nm);
        return;
    }
    if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    {
        error(av, ERR_ALLOCATION, "Error mapping file.", c->is_nm);
        return;
    }
    c->add(ptr, buf.st_size, av, c);
    return;
}

static uint32_t parse_flags(char *av, size_t i, t_context *c)
{
    uint32_t flags;

    flags = 0x00000000;
    if (i == 0 && av[i] == '-')
        return (0);
    if (!ft_strchr(c->flag_options, av[i]))
        flags |= UNKNOWN_FLAG;
    else if (av[i] == 'a')
        flags |= ALL;
    else if (av[i] == 'j')
        flags |= SYM_NAME_ONLY;
    else if (av[i] == 'u')
        flags |= UNDEF_ONLY;
    else if (av[i] == 'U')
        flags |= NO_UNDEF;
    else if (av[i] == 'r')
        flags |= SORT_REVERSE;
    else if (av[i] == 'A')
        flags |= PRINT_PATHNAME;
    flags |= parse_flags(av, i - 1, c);
    return flags;
}

int verify_flags(int ac, char *av[], t_context *c)
{
    while (--ac > 0)
    {
        if (av[ac][0] == '-')
        {
            c->flags |= parse_flags(av[ac], ft_strlen(av[ac])-1, c);
            if (c->flags & UNKNOWN_FLAG)
            {
                error(NULL, ERR_USAGE, av[ac], c->is_nm);
                return (ERR_USAGE);
            }
        } 
        else
            c->nfiles++;
    }
    return (EXIT_SUCCESS);
}