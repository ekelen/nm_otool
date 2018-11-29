/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:48:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/29 11:22:46 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>
#include <assert.h>

uint32_t parse_flags_nm(char *av, int *err, size_t i)
{
    uint32_t flags;

    if (i == 0 && av[i] == '-')
        return (0);
    else if (i == 0 && av[i] != '-')
        return ((*err = 2) && 0);
    else if (i > 0 && !ft_strchr(STR_FLAGS, av[i]))
        return ((*err = 2) && 0);
    flags = 0x00000000;
    if (av[i] == 'a')
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
    flags |= parse_flags_nm(av, err, i - 1);
    return flags;
}

int read_file_nm(uint32_t flags, char *av)
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

    return (add_file_nm(ptr, buf.st_size, av, flags));
}

int main(int argc, char *argv[])
{
    int err;
    size_t i;
    uint32_t flags;
    size_t nfiles;
    
    //TODO: a.out if no file
    //TODO: multiple files
    err = 0;
    flags = 0;
    i = argc;
    nfiles = 0;
    
    if (argc < 2)
        return (error("No file", 3));

    while (--i > 0)
    {
        if (argv[argc - i][0] == '-')
        {
            flags |= parse_flags_nm(argv[argc - i], &err, ft_strlen(argv[argc - i])-1);
            if (err)
                return(error(argv[argc - i], 2));
        }
    }

    i = argc;
    while (--i > 0)
    {
        if (argv[argc - i][0] != '-' && (++nfiles))
            err = read_file_nm(flags, argv[argc - i]);
    }
    if (!nfiles)
        return(read_file_nm(flags, "a.out"));
    
    return (0);
}