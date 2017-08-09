/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 20:26:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/09 02:56:14 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static char	use_buf(char *str)
{
	if (!ft_strcmp(str, SECT_TEXT))
		return ('T');
	if (!ft_strcmp(str, SECT_DATA))
		return ('D');
	if (!ft_strcmp(str, SECT_BSS))
		return ('B');
	return ('S');
}

char		symbol(size_t n_type, int value, char *buf)
{
	size_t	tmp;
	char	r;

	tmp = n_type;
	r = 0;
	n_type = n_type & N_TYPE;
	if (n_type == N_UNDF || n_type == N_PBUD)
	{
		if (value)
			r = 'C';
		else
			r = 'U';
	}
	else if (n_type == N_ABS)
		r = 'A';
	else if (n_type == N_INDR)
		r = 'I';
	else if (n_type == N_SECT)
		r = use_buf(buf);
	if (!(tmp & N_EXT) && r)
		r += 32;
	return (r);
}

static void	get_seg_name_64(struct segment_command_64 *segment, char **array)
{
	struct section_64	*sec;
	size_t				iter_array;
	size_t				iter_sec;

	iter_array = 0;
	iter_sec = 0;
	sec = (void *)((char *)segment + sizeof(struct segment_command_64));
	while (array[iter_array])
		++iter_array;
	while (iter_sec < segment->nsects)
	{
		array[iter_array] = sec[iter_sec].sectname;
		++iter_array;
		++iter_sec;
	}
	array[iter_array] = NULL;
}

void		swap_64(char **array, uint32_t nb)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	j = nb;
	while (i < j)
	{
		if (array[i] == NULL)
		{
			while (array[j] == NULL)
				j--;
			array[i] = ft_strdup(array[j]);
			ft_strdel(&array[j]);
		}
		++i;
	}
	i = 0;
	while (array[i])
		++i;
	if (!array[i] && array[i + 1] && (i + 1) <= nb)
	{
		array[i] = ft_strdup(array[i + 1]);
		ft_strdel(&array[i + 1]);
	}
}

#define I tab[0]
#define NCMDS tab[1]

void		handle_64(char *ptr)
{
	struct load_command		*lc;
	struct symtab_command	*sym;
	struct mach_header_64	*header;
	uint32_t				tab[2];
	char					*segment_name[4096];

	I = 0;
	sym = NULL;
	header = (void *)ptr;
	NCMDS = header->ncmds;
	lc = (void *)ptr + sizeof(struct mach_header_64);
	ft_memset(segment_name, 0, 4096);
	if (header->filetype > 0xb)
		return ;
	while (I < NCMDS)
	{
		if (lc->cmd == LC_SYMTAB)
			sym = (struct symtab_command *)lc;
		if (lc->cmd == LC_SEGMENT_64)
			get_seg_name_64((struct segment_command_64 *)lc, segment_name);
		lc = (void *)lc + lc->cmdsize;
		++I;
	}
	if (sym)
		print_output_64(sym, ptr, segment_name);
}
