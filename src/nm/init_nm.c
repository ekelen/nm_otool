/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_nm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 21:45:57 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/29 18:11:19 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>
#include <assert.h>

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