/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:47:06 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/28 14:10:13 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
#define NM_H

# define STR_FLAGS "ajuUr"

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

#endif