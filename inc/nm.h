/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:47:06 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/28 21:41:05 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
#define NM_H

#include "nm_otool.h"

# define STR_FLAGS "ajuUr"

# define SHOW_VALUE_MASK 0x1
# define SHOW_TYPE_MASK 0x2
# define SHOW_NAME_MASK 0x4
# define SHOW_STRVAL_MASK 0x8
# define SHOW_VAL_COL_MASK 0x10
# define SHOW_ANY_MASK 0x20

# define ALL_MASK 0x1
# define SYMDEF_NAME_ONLY_MASK 0x2
# define UNDEF_ONLY_MASK 0x4
# define NO_UNDEF_MASK 0x8
# define SORT_REVERSE_MASK 0x10

typedef enum e_user_flags {
    ALL = 1 << 0,
    SYM_NAME_ONLY = 1 << 1,
    UNDEF_ONLY = 1 << 2,
    NO_UNDEF = 1 << 3,
    SORT_REVERSE = 1 << 4 
}			t_e_user_flags;

typedef enum e_sym_print {
    SHOW_VALUE = 1 << 0,
    SHOW_TYPE = 1 << 1,
    SHOW_NAME = 1 << 2,
    SHOW_STRVAL = 1 << 3,
    SHOW_VAL_COL = 1 << 4, 
    SHOW_ANY = 1 << 5
}			t_e_sym_print;

// print_nm.c
void print_meta_statlib(t_file *file, t_mach_o *m);
void print_meta_single(t_file *file, t_mach_o *m);
void print_meta_fat(t_file *file, t_mach_o *m);
void print_machs(t_file *file, t_mach_o *m);

#endif