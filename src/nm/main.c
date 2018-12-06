/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:48:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/06 13:56:03 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void get_symbol_sort_nm(t_file *file, uint32_t flags)
{
	if (flags & SORT_REVERSE)
		file->sort = cmp_name_reverse;
	else
		file->sort = cmp_name;
	return;
}

static void add_file_nm(void *p, off_t s, char *av, t_context *c)
{
	t_file              *file;
    t_e_errs            err;

    err = SUCCESS;
    file = NULL;
    if (!(file = (t_file *)malloc(sizeof(t_file))))
    {
        error(av, ERR_ALLOCATION, NULL, c->is_nm);
        return;
    }
	if ((err = init_file(file, p, s, av)) > SUCCESS)
    {
        error(av, err, NULL, c->is_nm);
        return;
    }
    file->flags = c->flags;
    file->info |= 1 << 6;
	get_symbol_sort_nm(file, c->flags);
	if ((err = process_file(file, s)) > SUCCESS)
        error(av, err, NULL, c->is_nm);
    else
    {
        if (c->nfiles > 1 && file->info & IS_SINGLE_MACH && \
            (!(file->flags & PRINT_PATHNAME)))
            ft_printf("\n%s:\n", av);
        print_machs(file, file->mach);
    }
    free_file(file);
    return;
}

static t_context init_context(void)
{
    t_context c;
    c.is_nm = TRUE;
    c.flags = 0x00000000;
    c.err = 0;
    c.nfiles = 0;
    c.add = add_file_nm;
    c.flag_options = NM_FLAGS;
    return c;
}

int main(int argc, char *argv[])
{
    int             i;
    t_context       nmc;

    nmc = init_context();
    i = 0;
    if (verify_flags(argc, argv, &nmc) > SUCCESS)
        return (0);

    while (++i < argc)
    {
        if (argv[i][0] != '-')
            read_file(&nmc, argv[i]);
    }
    if (!nmc.nfiles)
        read_file(&nmc, "a.out");
    return (0);
}