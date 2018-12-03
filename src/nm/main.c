/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:48:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/03 11:33:44 by ekelen           ###   ########.fr       */
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

static void get_symbol_sort_nm(t_file *file, uint32_t flags)
{
	if (flags & SORT_REVERSE)
		file->sort = cmp_name_reverse;
	else
		file->sort = cmp_name;
	return;
}

void add_file_nm(void *data, off_t size, char *argname, t_context *nmc)
{
	t_file      *file;
    int         err;

    err = 0;
    if (!(file = (t_file *)malloc(sizeof(t_file))))
        err = ERR_ALLOCATION;
	if (!err && (err = init_file(file, data, size, argname)) > SUCCESS)
        err = ERR_FILE;
    file->flags = nmc->flags;
	get_symbol_sort_nm(file, nmc->flags);
	if (!err && (err = process_file(file, size)) > SUCCESS)
        error(argname, err);
    else
        print_machs(file, file->mach);
    free_file(file);
}

void read_file_nm(t_context *nmc, char *av)
{
    int fd;
    struct stat buf;
    void *ptr;

    if (nmc->nfiles > 1)
        ft_printf("\n%s:\n", av);
    if ((fd = open(av, O_RDONLY)) < 0)
        nmc->err = (error(av, 1));
    if (!nmc->err && fstat(fd, &buf) < 0)
        nmc->err = (error(av, 1));
    if (!nmc->err && (ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        nmc->err = (error(av, 1));
    if (!nmc->err)
        add_file_nm(ptr, buf.st_size, av, nmc);
    return;
}

int check_for_flags(int argc, char *argv[], t_context *nmc)
{
    while (--argc > 0)
    {
        if (argv[argc][0] == '-')
        {
            nmc->flags |= parse_flags_nm(argv[argc], &(nmc->err), ft_strlen(argv[argc])-1);
            if (nmc->err)
                return(error(argv[argc], nmc->err));
        } 
        else
            nmc->nfiles++;
    }
    return (EXIT_SUCCESS);
}

t_context init_context(void)
{
    t_context nmc;
    nmc.is_nm = TRUE;
    nmc.flags = 0x00000000;
    nmc.err = 0;
    nmc.nfiles = 0;
    return nmc;
}

int main(int argc, char *argv[])
{
    int             i;
    t_context       nmc;

    nmc = init_context();
    i = 0;
    check_for_flags(argc, argv, &nmc);
    if (nmc.err)
        return (nmc.err);

    while (++i < argc)
    {
        if (argv[i][0] != '-')
            read_file_nm(&nmc, argv[i]);
    }
    if (!nmc.nfiles)
        read_file_nm(&nmc, "a.out");
    return (0);
}