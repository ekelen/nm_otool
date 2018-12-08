/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 10:48:45 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/08 11:08:04 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int		cmp_nom(t_sym *sym1, t_sym *sym2)
{
	return (ft_strcmp(sym1->nom, sym2->nom));
}

static int		cmp_type(t_sym *sym1, t_sym *sym2)
{
	if (ft_strchr("Uu", sym2->type) && ft_strchr("Uu", sym1->type))
		return (cmp_nom(sym1, sym2));
	return (ft_strchr("Uu", sym2->type) - ft_strchr("Uu", sym1->type));
}

int64_t			cmp_value(t_sym *sym1, t_sym *sym2, bool r)
{
	int64_t	cmp;
	int		cmp_mod;

	cmp_mod = r ? -1 : 1;
	cmp = ((int64_t)sym1->n_value - (int64_t)sym2->n_value);
	if (cmp != 0)
		return (cmp * cmp_mod);
	cmp = (int64_t)cmp_type(sym1, sym2);
	return (cmp * cmp_mod);
}

int64_t			cmp_name(t_sym *sym1, t_sym *sym2, bool r)
{
	int64_t	cmp;
	int		cmp_mod;

	cmp_mod = r ? -1 : 1;
	cmp = (int64_t)cmp_nom(sym1, sym2);
	if (cmp != 0)
		return (cmp * cmp_mod);
	cmp = ((int64_t)sym1->n_value - (int64_t)(sym2->n_value));
	return (cmp * cmp_mod);
}

void			sort_symbol(int64_t (*sort)(t_sym *s1, t_sym *s2, bool r), \
	t_sym **curr, \
	t_sym *new, bool r)
{
	if (!sort)
		return ;
	if (!*curr)
	{
		*curr = new;
		return ;
	}
	if (sort((*curr), new, r) > 0)
		return (sort_symbol(sort, &(*curr)->left, new, r));
	else
		return (sort_symbol(sort, &(*curr)->right, new, r));
}
