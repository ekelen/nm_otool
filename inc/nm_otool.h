/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:47:06 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/06 08:28:21 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
#define NM_OTOOL_H

#include <libft.h>
#include <ft_printf.h>

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
# include <mach/machine.h>
# include <stdbool.h>

# include "nm.h"
# include "otool.h"

# define IS_64 0x1
# define IS_SWAP 0x2
# define IS_SINGLE_MACH 0x4
# define IS_FAT 0x8
# define IS_STATLIB 0x10
# define IS_MULTI 0x20
# define IS_NM 0x40

# define TEXT_SECT 0x000000ff
# define DATA_SECT 0x0000ff00
# define BSS_SECT 0x00ff0000

# define NM_NAME "ft_nm: "
# define OTOOL_NAME "ft_otool: "

# define HOST_CPU CPU_TYPE_X86_64

# define CIGAM_MASK 0xf0ffffff
# define MAGIC_MASK 0xfffffff0
# define SWAP_MAGIC (MH_CIGAM & CIGAM_MASK)
# define MH_ANY (MH_MAGIC & MAGIC_MASK)
# define SWAP_FAT (FAT_CIGAM & CIGAM_MASK)
# define FAT_ANY (FAT_MAGIC & MAGIC_MASK)
# define SWAP_ANY (SWAP_MAGIC)||(SWAP_FAT)

# define AR_HDR_SIZE sizeof(struct ar_hdr)
# define NLIST_SIZE sizeof(struct nlist)
# define NLIST_64_SIZE sizeof(struct nlist_64)
# define MH_SIZE sizeof(struct mach_header)

# define MH_64_SIZE sizeof(struct mach_header_64)
# define CMD_SIZE sizeof(struct segment_command)
# define CMD_64_SIZE sizeof(struct segment_command_64)
# define SEC_SIZE sizeof(struct section)
# define SEC_64_SIZE sizeof(struct section_64)
# define FH_SIZE sizeof(struct fat_arch)
# define FH_64_SIZE sizeof(struct fat_arch_64)
# define LC_SIZE sizeof(struct load_command)
# define SYMTAB_SIZE sizeof(struct symtab_command)

typedef unsigned char               t_byte;

typedef struct mach_header          t_mach_header;
typedef struct mach_header_64       t_mach_header_64;
typedef struct segment_command      t_segment_command;
typedef struct segment_command_64   t_segment_command_64;
typedef struct section              t_section;
typedef struct section_64           t_section_64;
typedef struct nlist                t_nlist;
typedef struct nlist_64             t_nlist_64;
typedef struct fat_arch             t_fat_arch;
typedef struct fat_arch_64          t_fat_arch_64;
typedef struct load_command         t_load_command;
typedef struct symtab_command       t_symtab_command;
typedef struct ranlib               t_ranlib;
typedef struct ar_hdr               t_ar_hdr;

typedef struct s_mach_o t_mach_o;
typedef struct s_ofile t_ofile;
typedef struct s_file t_file;
typedef struct s_symbol t_symbol;
typedef struct s_arch t_arch;

typedef union   u_u_sec {
    t_section      *sc32;
    t_section_64   *sc64;
}               t_u_sec;

typedef union   u_u_fa {
    t_fat_arch      *fa32;
    t_fat_arch_64   *fa64;
}               t_u_fa;

typedef union   u_u_nl {
    t_nlist      *nl32;
    t_nlist_64   *nl64;
}               t_u_nl;

typedef union   u_u_mh {
    t_mach_header      *mh32;
    t_mach_header_64   *mh64;
}               t_u_mh;

typedef struct s_arch_info t_arch_info;
struct s_arch_info {
    const char *name;
    cpu_type_t cpu_type;
    cpu_subtype_t cpu_subtype;
};

typedef enum e_errs t_e_errs;
enum e_errs {
    SUCCESS = 0,
    ERR_FILE = 1,
    ERR_USAGE = 2,
    ERR_OTHER = 3,
    ERR_ALLOCATION = 4
};

struct  s_symbol {
    char            *nom;
    const void      *nptr;
    char            type;
    uint8_t         n_type;
    uint64_t        n_value;
    uint8_t         n_sect;
    bool            m64;
    t_symbol        *left;
    t_symbol        *right;
};

typedef struct s_sec {
    size_t                  index;
    const void              *sec;
    t_u_sec                 sc;
    struct s_sec            *next;
}               t_sec;

struct s_arch {
    size_t                  offset; // sizeof fat_arch/fat_arch_64
    t_arch_info             arch_info;
    uint64_t                fa_offset;
    uint64_t                size;
};

// ofile.h
// TODO: Too verbose
struct s_ofile {
    const char *addr;      	    /* pointer to the member contents */
    uint32_t size;           /* actual size of the member (not rounded)*/
    const t_ar_hdr *hdr;   /* pointer to the ar_hdr for this member */
    const char *name;		            /* name of this member */
    uint32_t name_size;      /* size of the member name */
};

// TODO: Remove redundant fields
struct s_mach_o {
    uint32_t                magic;
    t_symbol                *symbols;
    bool                    m64;
    bool                    swap;
    const void              *data;

    size_t                  offset; // from mach header
    size_t                  nsyms;

    void                    *end; // data + length

    const void              *end_commands; // end of load commands

    t_mach_o                *next;

    uint64_t                nsects;
    uint8_t                 current_sect;

    t_symtab_command        *st;

    const void              *text_sect;

    t_arch                  arch;
    cpu_type_t              cputype;
    t_ofile                 ofile;
    void                    (*print_meta)(t_file *file, t_mach_o *m);
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
    t_mach_o                *mach; 
    int                     (*sort)(t_symbol *sym1, t_symbol *sym2);
};

typedef struct  s_context t_context;
struct          s_context {
    bool                    is_nm;
    uint32_t                flags;
    const char              *flag_options;
	int						err;
	size_t					nfiles;
    void                    (*add)(void *p, off_t s, char *av, t_context *c);
};

// util.c
uint32_t    nswap32(uint32_t x);
uint64_t    nswap64(uint64_t x);
uint64_t    swap64(uint64_t x);
uint32_t    swap32 (uint32_t x);
void        *ptr_check(const void *addr_max, void *req, size_t req_length);
void        *ptr_check_msg(const void *addr_max, void *req, size_t req_length, const char *msg);

//mach.c
int		            init_mach_o(t_file *file, const void *data, size_t size, t_mach_o *m);
int                 add_mach(t_mach_o **curr, t_mach_o *new);
void                remove_mach(t_mach_o *m);
void                free_machs(t_mach_o *curr);

// read_file.c
int                 process_file(t_file *file, size_t size);
int                 handle_32_64(t_file *file, size_t size);
void                read_file(t_context *c, char *av);
int                 verify_flags(int ac, char *av[], t_context *c);

// static_lib.c
int handle_archive(t_file *file);

// fat.c
t_arch              init_arch(t_file *file, t_u_fa f);
t_arch_info         get_arch_info(cpu_type_t cputype, cpu_subtype_t cpusubtype);
int                 handle_fat(t_file *file);

// parse_commands.c
int parse_seg(t_file *file, t_mach_o *m, const struct load_command *lc);

// parse_symtab.c
int parse_symtab(t_file *file, t_mach_o *m, const struct load_command *cmd);


// symbol.c
int add_symbol(t_file *file, t_mach_o *m, t_symtab_command *st, const void *nptr);
int cmp_name_reverse(t_symbol *sym1, t_symbol *sym2);
int cmp_name(t_symbol *sym1, t_symbol *sym2);
void free_symbols(t_symbol *curr);


// section.c
int get_secs(t_file *file, t_mach_o *m, void *seg, uint32_t nsects);

// file.c
int                 init_file(t_file *file, void *data, off_t size, char *argname);
void                free_file(t_file *file);

//print.c
void get_meta_print(t_file *file, t_mach_o *m);

//print_nm.c
void print_machs(t_file *file, t_mach_o *m);

//print.c (otool)
void print_otool(t_file *file, t_mach_o *m);

//error.c
int error(const char *filename, t_e_errs err, const char *msg, bool is_nm);


#endif