/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:47:06 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/04 17:54:47 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
#define NM_H

#include "nm_otool.h"

# define STR_FLAGS "AajuUr"

# define SHOW_VALUE 0x1
# define SHOW_TYPE 0x2
# define SHOW_NAME 0x4
# define SHOW_STRVAL 0x8
# define SHOW_VAL_COL 0x10
# define SHOW_ANY 0x20
# define SHOW_PATHNAME 0x40
# define SHOW_DEFAULT (SHOW_VALUE | SHOW_TYPE | SHOW_NAME | SHOW_VAL_COL | SHOW_ANY )

# define ALL 0x1
# define SYM_NAME_ONLY 0x2
# define UNDEF_ONLY 0x4
# define NO_UNDEF 0x8
# define SORT_REVERSE 0x10
# define PRINT_PATHNAME 0x20

// print_nm.c
// void print_meta_statlib(t_file *file, t_mach_o *m);
// void print_meta_single(t_file *file, t_mach_o *m);
void print_meta_fat(t_file *file, t_mach_o *m);
void print_machs(t_file *file, t_mach_o *m);

// init_nm.c
// int add_file_nm(void *data, off_t size, char *argname, uint32_t flags);

#endif