/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:47:06 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 15:09:39 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include <libft.h>
# include <ft_printf.h>

# include "constants.h"
# include "structures.h"

# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <ar.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/stab.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <stdbool.h>

/*
** util_swap.c
*/

uint32_t	nswap32(uint32_t x);
uint64_t	nswap64(uint64_t x);
uint64_t	swap64(uint64_t x);
uint32_t	swap32 (uint32_t x);


/*
** util_mem.c
*/

void		*ptr_check(const void *end, void *req, size_t len);
void		*ptr_check_msg(const void *end, void *req, size_t len, \
	const char *msg);
void		free_symbols(t_sym *curr);


/*
** mach.c
*/

int			init_mach_o(t_file *file, const void *data, size_t size, \
	t_m *m);
int			add_mach(t_m **curr, t_m *new);
void		remove_mach(t_m *m);
void		free_machs(t_m *curr);


/*
** read_file.c
*/

int			process_file(t_file *file, size_t size);
int			handle_32_64(t_file *file, size_t size);
void		read_file(t_context *c, char *av);
int			verify_flags(int ac, char *av[], t_context *c);

/*
** static_lib.c
*/

int			handle_statlib(t_file *file);

/*
** fat.c
*/

t_arch		init_arch(t_file *file, t_u_fa f);
t_arch_info	get_arch_info(cpu_type_t cputype, cpu_subtype_t cpusubtype);
int			handle_fat(t_file *file);

/*
** parse_commands.c
*/

int			get_segments(t_file *file, t_m *m);


/*
** parse_symtab.c
*/

int			parse_symtab(t_file *file, t_m *m, const struct load_command *cmd);


/*
** symbol.c
*/

int			add_symbol(t_file *file, t_m *m, struct symtab_command *st, \
	const void *nptr);

/*
** sort_symbol.c
*/

int64_t		cmp_name(t_sym *sym1, t_sym *sym2, bool r);
int64_t		cmp_value(t_sym *sym1, t_sym *sym2, bool r);
void		sort_symbol(int64_t (*sort)(t_sym *s1, t_sym *s2, bool r), \
	t_sym **curr, \
	t_sym *new, bool r);


/*
** section.c
*/

int			get_secs(t_file *file, t_m *m, void *seg, uint32_t nsects);

/*
** file.c
*/

int			init_file(t_file *file, void *data, off_t size, char *argname);
void		free_file(t_file *file);

/*
** print.c
*/

void		get_meta_print(t_file *file, t_m *m);

/*
** print_nm.c
*/

void		print_machs(t_file *file, t_m *m);

/*
** error.c
*/

int			error(const char *filename, t_status err, const char *msg, \
	bool is_nm);

/*
** print_otool.c
*/

void		print_otool(t_file *file, t_m *m);

#endif
