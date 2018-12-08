/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 13:36:14 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 13:43:59 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <ar.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/stab.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <stdbool.h>

typedef struct s_file			t_file;
typedef struct s_m				t_m;

typedef union			u_u_sec {
	struct section		*sc32;
	struct section_64	*sc64;
}						t_u_sec;

typedef union			u_u_fa {
	struct fat_arch			*fa32;
	struct fat_arch_64		*fa64;
}						t_u_fa;

typedef union			u_u_nl {
	struct nlist		*nl32;
	struct nlist_64		*nl64;
}						t_u_nl;

typedef union			u_u_mh {
	struct mach_header		*mh32;
	struct mach_header_64	*mh64;
}						t_u_mh;

typedef struct			s_arch_info {
	const char			*name;
	cpu_type_t			cpu_type;
	cpu_subtype_t		cpu_subtype;
}						t_arch_info;

typedef struct			s_sym {
	char			*nom;
	const void		*nptr;
	char			type;
	uint8_t			n_type;
	uint64_t		n_value;
	uint8_t			n_sect;
	bool			m64;
	struct s_sym	*left;
	struct s_sym	*right;
}						t_sym;

typedef struct			s_arch {
	size_t				offset;
	t_arch_info			arch_info;
	uint64_t			fa_offset;
	uint64_t			size;
}						t_arch;

typedef struct			s_ofile {
	const char				*addr;
	uint32_t				size;
	const struct ar_hdr		*hdr;
	const char				*name;
	uint32_t				name_size;
}						t_ofile;

struct					s_m {
	t_sym					*symbols;
	bool					m64;
	bool					swap;
	void					*data;
	size_t					offset;
	size_t					nsyms;
	void					*end;
	const void				*end_cmds;
	t_m						*next;
	uint64_t				nsects;
	uint8_t					current_sect;
	struct symtab_command	*st;
	void					*text_sect;
	t_arch					arch;
	cpu_type_t				cputype;
	t_ofile					ofile;
	void					(*print_meta)(struct s_file *file, struct s_m *m);
	uint32_t				(*swap32)(uint32_t x);
	uint64_t				(*swap64)(uint64_t x);
};

struct					s_file {
	uint32_t				info;
	const void				*data;
	const void				*end;
	size_t					length;
	const char				*filename;
	uint32_t				flags;
	uint32_t				(*swap32)(uint32_t x);
	uint64_t				(*swap64)(uint64_t x);
	size_t					offset;
	struct s_m				*mach;
	int64_t					(*sort)(t_sym *sym1, t_sym *sym2, bool r);
};

typedef struct			s_context {
	bool				is_nm;
	uint32_t			flags;
	const char			*flag_options;
	int					err;
	size_t				nfiles;
	void				(*add)(void *p, off_t s, char *av, struct s_context *c);
}						t_context;

#endif
