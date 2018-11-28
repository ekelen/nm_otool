/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_nm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 21:45:57 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/28 22:24:44 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>
#include <assert.h>

void get_meta_print_nm(t_file *file)
{
	file->print_meta = file->is_fat 
			? print_meta_fat
			: (file->is_statlib 
				? print_meta_statlib
				: print_meta_single);
	return;
}

void get_symbol_sort_nm(t_symbol *s, uint32_t flags)
{
	if ((flags & SORT_REVERSE_MASK) == SORT_REVERSE)
		s->sort = cmp_name_reverse;
	else
		s->sort = cmp_name;
	return;
}

// TODO: Put in common
int handle_file(t_file *file, size_t size)
{
	int result;
    if (file->is_fat)
        result = handle_fat(file);
    else if (file->is_statlib)
        result = handle_archive(file);
    else if (!file->is_statlib && !file->is_fat)
        result = ((file->mach = init_mach_o(file, file->data, size)) == NULL 
            ? EXIT_FAILURE
            : EXIT_SUCCESS);
	return result;
}

int add_file_nm(void *data, off_t size, char *argname, uint32_t flags)
{
	t_file *file;
	int result;

	if (!(file = init_file(data, size, argname, flags)))
		return EXIT_FAILURE;
	get_meta_print_nm(file);
	result = (start_file(file, size, flags));
	if (result == EXIT_FAILURE)
        return (error_extended(argname, 1, "Input file error."));
    else
        print_machs(file, file->mach);
    return EXIT_SUCCESS;
}