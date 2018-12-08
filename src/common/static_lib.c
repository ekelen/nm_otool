/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 11:39:40 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 11:52:00 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** Add result to ar_hdr pointer to get position of mach-O hdr
*/

static int	get_size(const char *name)
{
	int		x;
	char	*word;

	word = ft_strchr(name, '/') + 1;
	x = ft_atoi(word);
	return (x);
}

/*
** get name of archive object file by looking after the position of ARFMAG
*/

static char	*get_ar_name(const char *name)
{
	int		len;

	len = ft_strlen(ARFMAG);
	return (ft_strstr(name, ARFMAG) + len);
}

/*
** store ar_hdr info along with each mach-o in archive
*/

int			add_ofile(t_file *file, void *ptr, const void *header)
{
	t_ofile		o;
	t_m			*m;
	t_status	res;

	if (!(m = (t_m *)malloc(sizeof(t_m))))
		return (ERR_ALLOCATION);
	o.hdr = (const t_ar_hdr *)header;
	o.name_size = (size_t)get_size(o.hdr->ar_name);
	o.size = (uint32_t)ft_atoi(o.hdr->ar_size);
	o.name = (const char *)get_ar_name(o.hdr->ar_name);
	o.addr = ptr
		+ sizeof(t_ar_hdr)
		+ o.name_size;
	if ((res = init_mach_o(file, o.addr, o.size - o.name_size, m)) > SUCCESS)
	{
		free(m);
		return (res);
	}
	m->ofile = o;
	return (add_mach(&(file->mach), m));
}

/*
** parse static lib header (see ar.h)
*/

static int	check_archive_ptrs(t_file *file, void **ptr, struct ar_hdr **hdr, \
	size_t *ar_size)
{
	t_status	res;

	if (!(*hdr = (struct ar_hdr *)ptr_check_msg(file->end, *ptr, AR_HDR_SIZE, \
		"ar_hdr")))
		return (ERR_FILE);
	if (ft_atoi((*hdr)->ar_size) <= 0)
		return (ERR_FILE);
	if ((res = add_ofile(file, *ptr, (void *)(*hdr))) > SUCCESS)
		return (res);
	*ar_size = ft_atoi((*hdr)->ar_size);
	return (SUCCESS);
}

/*
** initial parse of static library (see ar.h)
*/

int			handle_archive(t_file *file)
{
	struct ar_hdr	*header;
	void			*ptr;
	size_t			ar_size;
	t_status		res;

	ptr = (void *)file->data + file->offset;
	if (!(header = (struct ar_hdr *)ptr_check_msg(file->end, ptr, AR_HDR_SIZE, \
		"archive header")))
		return (ERR_FILE);
	ar_size = (size_t)ft_atoi(header->ar_size);
	if (!(ptr = ptr_check_msg(file->end, ptr + ar_size + AR_HDR_SIZE, 0, \
		"first ptr")))
		return (ERR_FILE);
	while (ptr < file->end)
	{
		if ((res = check_archive_ptrs(file, &ptr, &header, &ar_size)) > SUCCESS)
			return (res);
		if (!(ptr = ptr_check_msg(file->end, ptr + ar_size + AR_HDR_SIZE, 0, \
			"new ptr")))
			return (ERR_FILE);
	}
	return (EXIT_SUCCESS);
}
