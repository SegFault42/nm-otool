/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_32_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/08 05:22:22 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/08 05:25:03 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

char	*get_symbol(struct symtab_command *sym, char *ptr, char **array)
{
	struct nlist		*tab;
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

void	get_seg_name_32(struct segment_command *segment, char **array)
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

void	swap(char **array, uint32_t nb)
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

void	delete_same_value_2(char **array, uint32_t nb)
{
	uint32_t	i;

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

void	delete_same_value(char **array, uint32_t nb)
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
	delete_same_value_2(array, nb);
}
