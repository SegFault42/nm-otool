/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_64_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/09 01:55:19 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/09 03:55:44 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/nm.h"

#define I tab[0]
#define J tab[1]

bool	free_same_1(char **array, uint32_t *tab)
{
	if (array[I][0] != ' ' && array[J][0] == ' ')
		ft_strdel(&array[J]);
	else if (array[I][0] == ' ' && array[J][0] != ' ')
	{
		ft_strdel(&array[I]);
		return (false);
	}
	else
		ft_strdel(&array[J]);
	return (true);
}

void	free_same(char **array, uint32_t nb)
{
	uint32_t	tab[2];

	I = 0;
	J = 1;
	while (I < nb)
	{
		if (array[I] && array[I][17] == '0')
			ft_strdel(&array[I]);
		if (!array[I])
		{
			++I;
			continue ;
		}
		J = I;
		++J;
		while (J < nb)
		{
			if (array[I] && array[J] && ft_strequ(&array[I][19],
						&array[J][19]) == 1)
				if (free_same_1(array, tab) == false)
					continue ;
			++J;
		}
		++I;
	}
}
