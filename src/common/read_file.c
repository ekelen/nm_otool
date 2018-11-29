/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 09:14:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/29 09:28:54 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

int process_file(t_file *file, size_t size)
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
	return (result);
}