/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:48:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/29 18:16:12 by ekelen           ###   ########.fr       */
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

int add_nm_file_to_context(t_nm_context *nmc, t_file **curr, t_file *n)
{
    if (!*curr)
        return((((*curr)->next = n)) && 0);
    add_nm_file_to_context(nmc, (*curr)->next, n);
}

static void get_symbol_sort_nm(t_file *file, uint32_t flags)
{
	if (flags & SORT_REVERSE)
		file->sort = cmp_name_reverse;
	else
		file->sort = cmp_name;
	return;
}

t_file *add_file_nm(void *data, off_t size, char *argname, uint32_t flags)
{
	t_file *file;
	int result;

	if (!(file = init_file(data, size, argname, flags)))
		return NULL;
	get_symbol_sort_nm(file, flags);
	result = (process_file(file, size));
	if (result == EXIT_FAILURE)
        NULL;
    else
		return file;
    //     print_machs(file, file->mach);
	// free_file(file);
    // return EXIT_SUCCESS;
}

t_file *read_file_nm(t_nm_context *nmc, char *av)
{
    int fd;
    struct stat buf;
    void *ptr;
    int error;

    if ((fd = open(av, O_RDONLY)) < 0)
        error = (error_extended(av, 1, "Couldn't open file"));
    if (!error && fstat(fd, &buf) < 0)
        error = (error_extended(av, 1, "fstat failure"));
    if (!error && (ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        error = (error_extended(av, 1, "Couldn't allocate space with munmap"));

    return (error ? NULL : add_file_nm(ptr, buf.st_size, av, nmc->flags));
}

int check_for_flags(int argc, char *argv[], int *err, uint32_t *flags)
{
    while (--argc > 0)
    {
        if (argv[argc][0] == '-')
        {
            *flags |= parse_flags_nm(argv[argc], err, ft_strlen(argv[argc])-1);
            if (*err)
                return(error(argv[argc], *err));
        }
    }
    return (EXIT_SUCCESS);
}

t_nm_context *init_context(int err, uint32_t flags)
{
    t_nm_context *nmc;

    nmc = malloc(sizeof(t_nm_context));

    nmc->is_nm = TRUE;
    nmc->flags = flags;
    nmc->err = err;
    nmc->nfiles = 0;
    nmc->files = NULL;

    return nmc;
}

int main(int argc, char *argv[])
{
    int err;
    size_t i;
    uint32_t flags;
    size_t nfiles;

    err = 0;

    t_nm_context    *nmc;
    
    
    //TODO: a.out if no file
    //TODO: multiple files
    
    flags = 0;
    i = argc;
    nfiles = 0;
    
    if (argc < 2)
        return (error("No file", 3));

    check_for_flags(argc, argv, &err, &flags);
    if (err)
        return (err);

    nmc = init_context(err, flags);

    while (--argc > 0)
    {
        if (argv[argc][0] != '-' && (++(nmc->nfiles)))
            read_file_nm(nmc, argv[argc]);
    }
    if (!nmc->nfiles)
        read_file_nm(nmc, "a.out");
    
    return (0);
}