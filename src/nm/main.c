/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:48:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/26 15:59:12 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>
#include <assert.h>

static int start_file(void *ptr, size_t size, char *argname, uint32_t flags)
{
    int result;
    t_file *file;

    if (!(file = init_file((void *)ptr, size, argname, flags)))
        return EXIT_FAILURE;

    if (file->is_fat)
        result = handle_fat(file);
    else if (file->is_statlib)
        result = handle_archive(file);
    else if (!file->is_statlib && !file->is_fat)
        result = (((file->mach = init_mach_o(file, file->data, size))) == NULL 
            ? EXIT_FAILURE 
            : EXIT_SUCCESS);

    // result = init_nm(ptr, size, argname, flags);
    if (result == EXIT_FAILURE)
        return (error_extended(argname, 1, "Input file error."));
    else
        print_machs(file, file->mach);
    return EXIT_SUCCESS;
}

uint32_t parse_flags(char *av, int *err, size_t i)
{
    if (i == 0 && av[i] == '-')
        return (0);
    else if (i == 0 && av[i] != '-')
        return ((*err = 2) && 0);
    else if (i > 0 && !ft_strchr(STR_FLAGS, av[i]))
        return ((*err = 2) && 0);

    uint32_t flags = 0;

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

    flags |= parse_flags(av, err, i - 1);
    return flags;
}

int read_file(uint32_t flags, char *av)
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

    return(start_file(ptr, buf.st_size, av, flags));
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
            flags |= parse_flags(argv[argc - i], &err, ft_strlen(argv[argc - i])-1);
            if (err)
                return(error(argv[argc - i], 2));
        }
    }

    i = argc;
    while (--i > 0)
    {
        if (argv[argc - i][0] != '-' && (++nfiles))
            read_file(flags, argv[argc - i]);
    }
    if (!nfiles)
        return(read_file(flags, "a.out"));
    
    return (0);
}