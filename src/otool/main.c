/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:49:45 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 13:48:28 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void			add_file_otool(void *p, off_t s, char *av, t_context *c)
{
	t_file		*file;
	t_status	err;

	err = SUCCESS;
	file = NULL;
	if (!(file = (t_file *)malloc(sizeof(t_file))))
	{
		error(av, ERR_ALLOCATION, NULL, c->is_nm);
		return ;
	}
	if ((err = init_file(file, p, s, av)) > SUCCESS)
	{
		error(av, err, NULL, c->is_nm);
		return ;
	}
	file->info |= 0 << 6;
	if ((err = process_file(file, s)) > SUCCESS)
		error(av, err, NULL, c->is_nm);
	else
		print_otool(file, file->mach);
	free_file(file);
	return ;
}

static t_context	init_context(void)
{
	t_context c;

	c.is_nm = FALSE;
	c.flags = 0x00000000;
	c.err = 0;
	c.nfiles = 0;
	c.add = add_file_otool;
	c.flag_options = OT_FLAGS;
	return (c);
}

int					main(int ac, char *av[])
{
	int			i;
	t_context	c;

	c = init_context();
	i = 0;
	if (verify_flags(ac, av, &c) > SUCCESS)
		return (0);
	while (++i < ac)
	{
		if (av[i][0] != '-')
			read_file(&c, av[i]);
	}
	if (!c.nfiles)
		error(NULL, 3, "No file provided.", FALSE);
	return (0);
}
