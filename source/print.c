/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/08 06:46:12 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/09 04:05:14 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

char	*get_symbol_64(struct symtab_command *sym, char *ptr, char **array)
{
	struct nlist_64		*tab;
	size_t				index;
	char				*type;

	index = 0;
	tab = (void *)ptr + sym->symoff;
	type = (char *)malloc(sizeof(char) * sym->nsyms);
	ft_memset(type, 0, sym->nsyms);
	while (index < sym->nsyms)
	{
		type[index] = symbol(tab[index].n_type, tab[index].n_value,
				array[tab[index].n_sect - 1]);
		++index;
	}
	return (type);
}

void	delete_same_value_64(char **array, uint32_t nb)
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
		if (array[i] && array[i][19] != '_' && ft_strequ(&array[i][19],
					"dyld_stub_binder") == 0)
			ft_strdel(&array[i]);
		++i;
	}
	swap_64(array, nb);
}

void	stock_symbol_64(char symbol, char **output)
{
	char	test[2];

	test[0] = symbol;
	test[1] = 0;
	if (test[0] == 'T' && ft_strnequ(*output, "                ", 16) == 1)
		ft_memset(*output, '0', 16);
	ft_strcat(*output, test);
	ft_strcat(*output, " ");
}

void	stock_addr_64(uint64_t n_value, char **output)
{
	char	*hexa_itoa;

	hexa_itoa = NULL;
	if (n_value)
	{
		hexa_itoa = ft_hexa_itoa(n_value, 0);
		ft_strxcat(*output, "0", 16 - (int)ft_strlen(hexa_itoa));
		ft_strcat(*output, hexa_itoa);
		ft_strdel(&hexa_itoa);
	}
	else
		ft_strxcat(*output, " ", 16);
	ft_strcat(*output, " ");
}

void	sort_and_print_64(char **output, uint64_t nsyms)
{
	ft_sort_double_array(output);
	delete_same_value_64(output, nsyms);
	ft_sort_double_array(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
}
