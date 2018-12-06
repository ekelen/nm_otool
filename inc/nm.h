/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:47:06 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/06 13:48:43 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
#define NM_H

# define NM_FLAGS "AjuUr"

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
# define UNKNOWN_FLAG 0x40

#endif