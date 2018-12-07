/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 13:08:58 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/07 10:23:09 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

int		error(const char *filename, t_e_errs err, const char *msg, bool is_nm)
{
	ft_putstr_fd(is_nm ? NM_NAME : OTOOL_NAME, 2);
	if (err == ERR_USAGE)
	{
		ft_putstr_fd("Unknown flag ", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd(".  Valid flag(s): ", 2);
		ft_putstr_fd(is_nm ? NM_FLAGS : OT_FLAGS, 2);
	}
	else if (err == ERR_FILE)
	{
		ft_putstrcolon_fd(filename, 2, ' ');
		ft_putstr_fd(msg ? msg : "File is truncated or malformed.", 2);
	}
	else if (err == ERR_ALLOCATION)
	{
		ft_putstrcolon_fd(filename, 2, ' ');
		ft_putstr_fd(msg ? msg : "Error allocating heap space.", 2);
	}
	else if (err == ERR_OTHER && msg)
		ft_putstr_fd(msg, 2);
	ft_putendl_fd("", 2);
	return (err);
}
