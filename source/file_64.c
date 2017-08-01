/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 20:26:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/07/29 23:31:41 by rabougue         ###   ########.fr       */
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

char	symbol(size_t n_type, int value, char *buf)
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

static char	*get_symbol(struct symtab_command *sym, char *ptr, char **array)
{
	struct nlist_64		*tab;
	size_t				index;
	char				*type;

	index = 0;
	tab = (void *)ptr + sym->symoff;
	type = (char *)malloc(sizeof (char) * sym->nsyms);
	ft_memset(type, 0, sym->nsyms);
	while (index < sym->nsyms)
	{
		type[index] = symbol(tab[index].n_type, tab[index].n_value, array[tab[index].n_sect - 1]);
		++index;
	}
	return (type);
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

static void	swap_64(char **array, uint32_t nb)
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

static void	free_same(char **array, uint32_t nb)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	j = 1;
	while (i < nb)
	{
		if (array[i] && array[i][17] == '0')
			ft_strdel(&array[i]);
		if (!array[i])
		{
			++i;
			continue ;
		}
		j = i;
		++j;
		while (j < nb)
		{
			if (array[i] && array[j] && ft_strequ(&array[i][19], &array[j][19]) == 1)
			{
				if (array[i][0] != ' ' && array[j][0] == ' ')
					ft_strdel(&array[j]);
				else if (array[i][0] == ' ' && array[j][0] != ' ')
				{
					ft_strdel(&array[i]);
					continue ;
				}
				else
					ft_strdel(&array[j]);
			}
			++j;
		}
		++i;
	}
}

static void	delete_same_value_64(char **array, uint32_t nb)
{
	uint32_t	i;

	i = 0;
	free_same(array, nb);
	while (i < nb)
	{
		if (array[i] && array[i][19] == '/')
			ft_strdel(&array[i]);
		if (array[i] && ft_strlen(array[i]) == 19)
			ft_strdel(&array[i]);
		if (array[i] && array[i][19] != '_' && ft_strequ(&array[i][19], "dyld_stub_binder") == 0)
			ft_strdel(&array[i]);
		++i;
	}
	swap_64(array, nb);
}

static void	print_output_64(struct symtab_command *sym, struct mach_header_64 *header, char *ptr, char **segment_name)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;
	char			**output;
	char			*hexa_itoa;
	char			*symbol;
	char			test[2];

	i = 0;
	array = (void *)ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;
	output = (char **)malloc(sizeof(char *) * sym->nsyms + 1);
	if (!output)
		ft_critical_error(MALLOC_ERROR);
	output[sym->nsyms] = NULL;
	symbol = get_symbol(sym, ptr, segment_name);
	while (i < sym->nsyms)
	{
		output[i] = (char *)ft_memalloc(sizeof(char) * 19 + ft_strlen(stringtable + array[i].n_un.n_strx) + 1);
		if (!output)
			ft_critical_error(MALLOC_ERROR);
		if (array[i].n_value)
		{
			hexa_itoa = ft_hexa_itoa(array[i].n_value, 0);
			ft_strxcat(output[i], "0", 16 - (int)ft_strlen(hexa_itoa));
			ft_strcat(output[i], hexa_itoa);
			ft_strdel(&hexa_itoa);
		}
		else
			ft_strxcat(output[i], " ", 16);
		ft_strcat(output[i], " ");
		test[0] = symbol[i];
		test[1] = 0;
		if (test[0] == 'T' && ft_strnequ(output[i], "                ", 16) == 1)
			ft_memset(output[i], '0', 16);
		ft_strcat(output[i], test);
		ft_strcat(output[i], " ");
		ft_strcat(output[i], stringtable + array[i].n_un.n_strx); // stock le nom
		++i;
	}
	ft_strdel(&symbol);
	ft_sort_double_array(output);
	delete_same_value_64(output, sym->nsyms);
	ft_sort_double_array(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
}


void	handle_64(char *ptr)
{
	struct load_command		*lc;
	struct symtab_command	*sym;
	struct mach_header_64	*header;
	uint32_t				i;
	uint32_t				ncmds;
	char					*segment_name[4096];

	i = 0;
	sym = NULL;
	header = (void *)ptr; // header pointe sur ptr (octet 0 du binaire)
	ncmds = header->ncmds; // ncmds contient le nombre de load_command
	lc = (void *)ptr + sizeof(struct mach_header_64); // lc pointe sur le debut de la zone des load commands (juste apres le header)
	ft_memset(segment_name, 0, 4096);
	if (header->filetype > 0xb)
	{
		printf(RED"Invalid file\n"END);
		return ;
	}
	while (i < ncmds) // on iter autant de fois qu'il y a de load commands
	{
		if (lc->cmd == LC_SYMTAB) // si la cmd est egal a LC_SYMTAB
			sym = (struct symtab_command *)lc;
		if (lc->cmd == LC_SEGMENT_64)
			get_seg_name_64((struct segment_command_64 *)lc, segment_name);
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
	}
	if (sym)
		print_output_64(sym, header, ptr, segment_name);
}
