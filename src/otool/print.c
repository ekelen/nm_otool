/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 14:26:00 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/07 10:46:19 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void print_otool_addr(bool m64, void *addr, size_t i)
{
    if (m64)
        ft_putuint_base_pad((uintmax_t)((uint64_t)&(addr[i])), 16, 16, '0');
    else
        ft_putuint_base_pad((uintmax_t)((uint32_t)&(addr[i])), 16, 8, '0');
    ft_putchar('\t');
}

static void print_otool_spaced(t_m *m, uint64_t size, void *start, void *addr)
{
    size_t          i;
    size_t          j;

    i = 0;
    while (i < size)
    {
        print_otool_addr(m->m64, addr, i);
        j = 0;
		while (j < 16 && i + j < size)
		{
            ft_putuint_base_pad((*((uint8_t *)(&start[j]))), 16, 2, '0');
            ft_putchar(' ');
			j++;
		}
        ft_putendl("");
        start = (void *)start + 16;
        i += 16;
    }
}

static void print_otool_block(t_m *m, uint64_t size, void *start, void *addr)
{
    size_t          i;
    size_t          j;
    uint32_t        block;

    i = 0;
    while (i < size)
    {
        print_otool_addr(m->m64, addr, i);
        j = 0;
		while (j < 16 && i + j < size)
		{
			block = m->swap32((*(uint32_t *)(start + j)));
            ft_putuint_base_pad(block, 16, 8, '0');
            ft_putchar(' ');
			j += 4;
		}
        ft_putendl("");
        start = (void *)start + 16;
        i += 16;
    }
}

static void print_otool_32(t_m *m, struct section *text_sec)
{
	uint64_t    size;
    uint32_t    offset;
    void        *addr;

	size = (uint64_t)(m->swap32(text_sec->size));
	offset = m->swap32(text_sec->offset);
	addr = (void *)((uint64_t)(m->swap32(text_sec->addr)));
	if (m->cputype == CPU_TYPE_I386 || m->cputype == CPU_TYPE_X86_64)
		print_otool_spaced(m, size, (void *)m->data + offset, addr);
	else
		print_otool_block(m, size, (void *)m->data + offset, addr);
}

static void print_otool_64(t_m *m, struct section_64 *text_sec)
{
	uint64_t    size;
    uint32_t    offset;
    void        *addr;

	size = m->swap64(text_sec->size);
	offset = m->swap32(text_sec->offset);
	addr = (void *)(m->swap64(text_sec->addr));
	if (m->cputype == CPU_TYPE_I386 || m->cputype == CPU_TYPE_X86_64)
		print_otool_spaced(m, size, (void *)m->data + offset, addr);
	else
		print_otool_block(m, size, (void *)m->data + offset, addr);
}

void print_otool(t_file *file, t_m *m)
{
	if (!file || !file->mach || !m || !(m->nsects & TEXT_SECT))
        return;
	m->print_meta(file, m);
	if (m->m64)
		print_otool_64(m, (struct section_64 *)(m->text_sect));
	else
		print_otool_32(m, (struct section *)(m->text_sect));
	print_otool(file, m->next);
}