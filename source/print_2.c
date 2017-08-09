/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/09 04:03:28 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/09 04:04:28 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/nm.h"

#define LEN ft_strlen(stringtable + array[i].n_un.n_strx) + 1

void	print_output_64(struct symtab_command *sym, char *ptr,
		char **segment_name)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;
	char			**output;
	char			*symbol;

	i = 0;
	array = (void *)ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;
	if ((output = (char **)malloc(sizeof(char *) * sym->nsyms + 1)) == NULL)
		ft_critical_error(MALLOC_ERROR);
	output[sym->nsyms] = NULL;
	symbol = get_symbol_64(sym, ptr, segment_name);
	while (i < sym->nsyms)
	{
		output[i] = (char *)ft_memalloc(sizeof(char) * 19 + LEN);
		if (!output)
			ft_critical_error(MALLOC_ERROR);
		stock_addr_64(array[i].n_value, &output[i]);
		stock_symbol_64(symbol[i], &output[i]);
		ft_strcat(output[i], stringtable + array[i].n_un.n_strx);
		++i;
	}
	ft_strdel(&symbol);
	sort_and_print_64(output, sym->nsyms);
}
