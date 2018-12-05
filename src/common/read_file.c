/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 09:14:36 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/05 09:21:19 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

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