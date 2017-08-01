/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_32.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 18:28:29 by rabougue          #+#    #+#             */
/*   Updated: 2017/07/25 17:59:20 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static char	*get_symbol(struct symtab_command *sym, char *ptr, char **array)
{
	struct nlist		*tab;
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

static void	get_seg_name_32(struct segment_command *segment, char **array)
{
	struct section		*sec;
	size_t				iter_array;
	size_t				iter_sec;

	iter_array = 0;
	iter_sec = 0;
	sec = (void *)((char *)segment + sizeof(struct segment_command));
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

static void	swap(char **array, uint32_t nb)
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
}

static void	delete_same_value(char **array, uint32_t nb)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (i < nb)
	{
		while (!array[i])
			++i;
		j = i;
		++j;
		while (j < nb)
		{
			if ((array[j] && ft_strcmp(array[i], array[j]) == 0))
				ft_strdel(&array[j]);
			if ((array[j] && ft_strcmp(&array[i][11], &array[j][11]) == 0) &&
			ft_strncmp(array[i], "        ", 8) == 0)
			{
				ft_strdel(&array[i]);
				j = nb;
			}
			j++;
		}
		++i;
	}
	i = 0;
	while (i < nb)
	{
		if (array[i] && array[i][11] == '/')
			ft_strdel(&array[i]);
		if (array[i] && ft_strlen(array[i]) == 11)
			ft_strdel(&array[i]);
		if (array[i] && array[i][9] == '0')
			ft_strdel(&array[i]);
		++i;
	}
	swap(array, nb);
	swap(array, nb);
}

static void	print_output_32(struct symtab_command *sym, struct mach_header *header, char *ptr, char **segment_name)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist	*array;
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
		output[i] = (char *)ft_memalloc(sizeof(char) * 11 + ft_strlen(stringtable + array[i].n_un.n_strx) + 1);
		if (!output)
			ft_critical_error(MALLOC_ERROR);
		if (array[i].n_value)
		{
			hexa_itoa = ft_hexa_itoa(array[i].n_value, 0);
			ft_strxcat(output[i], "0", 8 - (int)ft_strlen(hexa_itoa));
			ft_strcat(output[i], hexa_itoa);
			ft_strdel(&hexa_itoa);
		}
		else
			ft_strxcat(output[i], " ", 8);
		ft_strcat(output[i], " ");

		test[0] = symbol[i];
		test[1] = 0;
		if (test[0])
		{
			ft_strcat(output[i], test);
			ft_strcat(output[i], " ");
		}
		else
			ft_strcat(output[i], "0 ");
		ft_strcat(output[i], stringtable + array[i].n_un.n_strx); // stock le nom
		++i;
	}
	ft_sort_double_array_32(output);
	delete_same_value(output, sym->nsyms);
	ft_sort_double_array_32(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
}

void	handle_32(char *ptr)
{
	struct load_command		*lc;
	struct symtab_command	*sym;
	struct mach_header		*header;
	uint32_t				i;
	uint32_t				ncmds;
	char					*segment_name[4096];

	i = 0;
	sym = NULL;
	header = (void *)ptr; // header pointe sur ptr (octet 0 du binaire)
	ncmds = header->ncmds; // ncmds contient le nombre de load_command
	lc = (void *)ptr + sizeof(struct mach_header); // lc pointe sur le debut de la zone des load commands (juste apres le header)
	if (header->filetype > 0xb)
	{
		printf(RED"Invalid file\n"END);
		return ;
	}
	while (i < ncmds) // on iter autant de fois qu'il y a de load commands
	{
		if (lc->cmd == LC_SYMTAB) // si la cmd est egal a LC_SYMTAB
			sym = (struct symtab_command *)lc;
		if (lc->cmd == LC_SEGMENT) // si la cmd est egal a LC_SYMTAB
			get_seg_name_32((struct segment_command *)lc, segment_name);
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
	}
	if (sym)
		print_output_32(sym, header, ptr, segment_name);
}
