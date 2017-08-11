/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_32.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 18:28:29 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/08 07:19:27 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	stock_addr(uint32_t n_value, char **output)
{
	char	*hexa_itoa;

	hexa_itoa = NULL;
	if (n_value)
	{
		hexa_itoa = ft_hexa_itoa(n_value, 0);
		ft_strxcat(*output, "0", 8 - (int)ft_strlen(hexa_itoa));
		ft_strcat(*output, hexa_itoa);
		ft_strdel(&hexa_itoa);
	}
	else
		ft_strxcat(*output, " ", 8);
	ft_strcat(*output, " ");
}

static void	stock_symbol(char symbol, char **output, uint32_t filetype)
{
	char	test[2];

	test[0] = symbol;
	test[1] = 0;
	if (test[0])
	{
		ft_strcat(*output, test);
		ft_strcat(*output, " ");
	}
	else
		ft_strcat(*output, "0 ");
	if (symbol == 'T' && filetype == MH_OBJECT)
		ft_memset(*output, '0', 8);
}

static void	sort_and_print(char **output, uint32_t nsyms)
{
	ft_sort_double_array_32(output);
	delete_same_value(output, nsyms);
	ft_sort_double_array_32(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
}

#define LEN ft_strlen(stringtable + array[i].n_un.n_strx) + 1

static void	print_output_32(struct symtab_command *sym, char *ptr,
		char **segment_name, uint32_t filetype)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist	*array;
	char			**output;
	char			*symbol;

	i = 0;
	array = (void *)ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;
	if ((output = (char **)malloc(sizeof(char *) * sym->nsyms + 1)) == NULL)
		ft_critical_error(MALLOC_ERROR);
	output[sym->nsyms] = NULL;
	symbol = get_symbol(sym, ptr, segment_name);
	while (i < sym->nsyms)
	{
		output[i] = (char *)ft_memalloc(sizeof(char) * 11 + LEN);
		if (!output[i])
			ft_critical_error(MALLOC_ERROR);
		stock_addr(array[i].n_value, &output[i]);
		stock_symbol(symbol[i], &output[i], filetype);
		ft_strcat(output[i], stringtable + array[i].n_un.n_strx);
		++i;
	}
	ft_strdel(&symbol);
	sort_and_print(output, sym->nsyms);
}

#define I tab[0]
#define NCMDS tab[1]

void		handle_32(char *ptr)
{
	struct load_command		*lc;
	struct symtab_command	*sym;
	struct mach_header		*header;
	uint32_t				tab[2];
	char					*segment_name[4096];

	I = 0;
	sym = NULL;
	header = (void *)ptr;
	NCMDS = header->ncmds;
	lc = (void *)ptr + sizeof(struct mach_header);
	ft_memset(segment_name, 0, 4096);
	if (header->filetype > 0xb)
		return ;
	while (I < NCMDS)
	{
		if (lc->cmd == LC_SYMTAB)
			sym = (struct symtab_command *)lc;
		if (lc->cmd == LC_SEGMENT)
			get_seg_name_32((struct segment_command *)lc, segment_name);
		lc = (void *)lc + lc->cmdsize;
		++I;
	}
	if (sym)
		print_output_32(sym, ptr, segment_name, header->filetype);
}
