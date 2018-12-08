/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 13:14:57 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 13:49:07 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# include "nm.h"
# include "otool.h"

# include <ar.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/stab.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>

# define IS_64 0x1
# define IS_SWAP 0x2
# define IS_SINGLE_MACH 0x4
# define IS_FAT 0x8
# define IS_STATLIB 0x10
# define IS_MULTI 0x20
# define IS_NM 0x40
# define IS_MULTI_AV 0x80

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
# define T_LC struct load_command

typedef enum	e_status {
	SUCCESS = 0,
	ERR_FILE = 1,
	ERR_USAGE = 2,
	ERR_OTHER = 3,
	ERR_ALLOCATION = 4
}				t_status;

#endif
