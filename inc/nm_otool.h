/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 10:47:06 by ekelen            #+#    #+#             */
/*   Updated: 2018/11/28 15:47:22 by ekelen           ###   ########.fr       */
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

#include "nm.h"

# define HOST_CPU CPU_TYPE_X86_64

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

typedef struct s_arch_info t_arch_info;
struct s_arch_info {
    char name[20];
    cpu_type_t cpu_type;
    cpu_subtype_t cpu_subtype;
};

typedef enum e_error t_e_error;
enum e_error {
    ERR_FILE = 1,
    ERR_USAGE = 2
};

typedef enum e_nsect t_e_nsect;
enum e_nsect {
    TEXT_SECT = 0x000000ff,
    DATA_SECT = 0x0000ff00,
    BSS_SECT = 0x00ff0000
};


// // type: char-encoded symbol types (per nm man pages)
// // sym_name: human-readable char string name
// // value: offset location in
struct  s_symbol {
    char            *nom;
    const void      *nptr;
    char            type;
    uint8_t         n_type;
    uint64_t        value;
    uint64_t        n_value;
    uint8_t         n_sect;
    void            (*print_value)(t_symbol *symbol);
    uint32_t        print;
    bool            m64;

    int             (*sort)(t_symbol *sym1, t_symbol *sym2);
    t_symbol        *left;
    t_symbol        *right;
};

// // index: which section it is (1-indexed)
// // sc: actual section struct
typedef struct s_sec {
    size_t                  index;
    const void              *sec;
    t_u_sec                 sc;
    struct s_sec            *next;
}               t_sec;

struct s_arch {
    bool                    m64;
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
    uint32_t                flags;
    uint32_t                magic;
    t_symbol                *symbols;
    bool                    m64;
    bool                    swap;
    const void              *data;

    size_t                  offset; // from mach header
    size_t                  nsyms;

    void                    *end; // data + length
    t_mach_o                *next;

    uint64_t                nsects;
    uint8_t                 current_sect;

    t_symtab_command        *st;

    t_sec                   *text_sect;

    t_arch                  arch;
    cpu_type_t              cputype;
    t_ofile                 ofile;

    uint32_t				(*swap32)(uint32_t x);
    uint64_t				(*swap64)(uint64_t x);
};



struct  s_file {
	bool					m64;
	bool					is_fat;
	bool					is_statlib;
    bool                    is_multi;
    bool                    swap;
	const void				*data;
    const void              *end;
	size_t					length;
    char                    *filename;
    uint32_t                flags;
	uint32_t				(*swap32)(uint32_t x);
    uint64_t				(*swap64)(uint64_t x);
    size_t                  offset;

    t_mach_o                *mach;

    void                    (*print_meta)(t_file *file, t_mach_o *m);
    // void                    (*print_otool_meta)(t_file *file, t_mach_o *m);
};

// util.c
uint32_t nswap32(uint32_t x);
uint64_t nswap64(uint64_t x);
uint64_t swap64(uint64_t x);
uint32_t swap32 (uint32_t x);
void *ptr_read(void *addr, size_t addr_len, const void *req, size_t req_length);
void *ptr_check(void *addr_max, const void *req, size_t req_length);
void *ptr_check_msg(void *addr_max, const void *req, size_t req_length, const char *msg);

//mach.c
t_mach_o            *init_mach_o(t_file *file, void *data, size_t size);
int                 add_mach(t_mach_o **curr, t_mach_o *new);

// static_lib.c
int handle_archive(t_file *file);

// fat.c
t_arch init_arch    (t_file *file, t_u_fa f);
t_arch_info         get_arch_info(cpu_type_t cputype, cpu_subtype_t cpusubtype);
int                 handle_fat(t_file *file);
char                *get_archname(cpu_type_t cputype, cpu_subtype_t cpusubtype);

// parse_commands.c
int parse_seg(t_file *file, t_mach_o *m, const struct load_command *lc);

// parse_symtab.c
int parse_symtab(t_file *file, t_mach_o *m, const struct load_command *cmd);

// symbol.c
int add_symbol(t_file *file, t_mach_o *m, t_symtab_command *st, const void *nptr);

// section.c
int get_secs(t_file *file, t_mach_o *m, void *seg, uint32_t nsects);
t_sec *find_sect(t_sec *curr, uint8_t i);

// init.c
t_file              *init_file(void *data, off_t size, char *argname, uint32_t flags);

//print.c
void print_in_order(t_symbol *head, t_symbol *current);
void print_nsecs(t_mach_o *m);
void print_secs(t_sec *curr);

void print_meta_statlib(t_file *file, t_mach_o *m);
void print_meta_single(t_file *file, t_mach_o *m);
void print_meta_fat(t_file *file, t_mach_o *m);
void print_symbols(t_file *file);

//error.c
int error(const char *arg, int err);
int error_extended(const char *arg, int err, const char *msg);

#endif