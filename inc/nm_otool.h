/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:47:06 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 13:26:37 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include <libft.h>
# include <ft_printf.h>

# include "nm.h"
# include "otool.h"
# include "constants.h"

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

typedef union   u_u_sec {
    struct section      *sc32;
    struct section_64   *sc64;
}               t_u_sec;

typedef union   u_u_fa {
    struct fat_arch      *fa32;
    struct fat_arch_64   *fa64;
}               t_u_fa;

typedef union   u_u_nl {
    struct nlist      *nl32;
    struct nlist_64   *nl64;
}               t_u_nl;

typedef union   u_u_mh {
    struct mach_header      *mh32;
    struct mach_header_64   *mh64;
}               t_u_mh;

typedef struct s_arch_info {
    const char *name;
    cpu_type_t cpu_type;
    cpu_subtype_t cpu_subtype;
}				t_arch_info;

typedef struct  s_sym {
    char            *nom;
    const void      *nptr;
    char            type;
    uint8_t         n_type;
    uint64_t        n_value;
    uint8_t         n_sect;
    bool            m64;
    struct s_sym	*left;
    struct s_sym	*right;
}				t_sym;

typedef struct s_arch {
    size_t                  offset; // sizeof fat_arch/fat_arch_64
    t_arch_info             arch_info;
    uint64_t                fa_offset;
    uint64_t                size;
}				t_arch;

typedef struct s_ofile {
    const char 				*addr;
    uint32_t 				size;
    const struct ar_hdr 	*hdr;
    const char 				*name;
    uint32_t 				name_size;
}				t_ofile;

typedef struct s_file t_file;
typedef struct s_m t_m;

struct s_m {
    t_sym                   *symbols;
	bool                    m64;
    bool                    swap;
    void              		*data;
    size_t                  offset;
    size_t                  nsyms;
    void					*end;
    const void              *end_cmds;
    t_m                		*next;
    uint64_t                nsects;
    uint8_t                 current_sect;
    struct symtab_command        *st;
    void              		*text_sect;
    t_arch                  arch;
    cpu_type_t              cputype;
    t_ofile                 ofile;
    void                    (*print_meta)(struct s_file *file, struct s_m *m);
    uint32_t				(*swap32)(uint32_t x);
    uint64_t				(*swap64)(uint64_t x);
};

struct  s_file {
    uint32_t                info;
	const void				*data;
    const void              *end;
	size_t					length;
    const char              *filename;
    uint32_t                flags;
	uint32_t				(*swap32)(uint32_t x);
    uint64_t				(*swap64)(uint64_t x);
    size_t                  offset;
	struct s_m				*mach; 
    int64_t                 (*sort)(t_sym *sym1, t_sym *sym2, bool r);
};


typedef struct			s_context {
    bool                    is_nm;
    uint32_t                flags;
    const char              *flag_options;
	int						err;
	size_t					nfiles;
    void                    (*add)(void *p, off_t s, char *av, struct s_context *c);
}						t_context;

// util.c
uint32_t    nswap32(uint32_t x);
uint64_t    nswap64(uint64_t x);
uint64_t    swap64(uint64_t x);
uint32_t    swap32 (uint32_t x);
void        *ptr_check(const void *addr_max, void *req, size_t req_length);
void        *ptr_check_msg(const void *addr_max, void *req, size_t req_length, const char *msg);

//mach.c
int		            init_mach_o(t_file *file, const void *data, size_t size, t_m *m);
int                 add_mach(t_m **curr, t_m *new);
void                remove_mach(t_m *m);
void                free_machs(t_m *curr);

// read_file.c
int                 process_file(t_file *file, size_t size);
int                 handle_32_64(t_file *file, size_t size);
void                read_file(t_context *c, char *av);
int                 verify_flags(int ac, char *av[], t_context *c);

// static_lib.c
int handle_statlib(t_file *file);

// fat.c
t_arch              init_arch(t_file *file, t_u_fa f);
t_arch_info         get_arch_info(cpu_type_t cputype, cpu_subtype_t cpusubtype);
int                 handle_fat(t_file *file);

// parse_commands.c
int    	get_segments(t_file *file, t_m *m);

// parse_symtab.c
int parse_symtab(t_file *file, t_m *m, const struct load_command *cmd);


// symbol.c
void free_symbols(t_sym *curr);
int add_symbol(t_file *file, t_m *m, struct symtab_command *st, const void *nptr);

// symbol_sort.c
int64_t cmp_name(t_sym *sym1, t_sym *sym2, bool r);
int64_t cmp_value(t_sym *sym1, t_sym *sym2, bool r);
void	sort_symbol(int64_t (*sort)(t_sym *s1, t_sym *s2, bool r), \
	t_sym **curr, \
	t_sym *new, bool r);


// section.c
int get_secs(t_file *file, t_m *m, void *seg, uint32_t nsects);

// file.c
int                 init_file(t_file *file, void *data, off_t size, char *argname);
void                free_file(t_file *file);

//print.c
void get_meta_print(t_file *file, t_m *m);

//print_nm.c
void print_machs(t_file *file, t_m *m);

//print.c (otool)
void print_otool(t_file *file, t_m *m);

//error.c
int error(const char *filename, t_status err, const char *msg, bool is_nm);


#endif