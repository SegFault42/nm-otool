/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_double_array.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/08 02:52:12 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/08 02:52:44 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

void	ft_sort_double_array_32(char **arguments)
{
	char	*tmp;
	int		find;
	int		i;

	find = 1;
	while (find)
	{
		find = 0;
		i = 0;
		while (arguments[i + 1])
		{
			if (ft_strcmp(&arguments[i][11], &arguments[i + 1][11]) > 0)
			{
				tmp = arguments[i];
				arguments[i] = arguments[i + 1];
				arguments[i + 1] = tmp;
				find = 1;
			}
			++i;
		}
	}
}

void	ft_sort_double_array(char **arguments)
{
	char	*tmp;
	int		find;
	int		i;

	find = 1;
	while (find)
	{
		find = 0;
		i = 0;
		while (arguments[i + 1])
		{
			if (ft_strcmp(&arguments[i][19], &arguments[i + 1][19]) > 0)
			{
				tmp = arguments[i];
				arguments[i] = arguments[i + 1];
				arguments[i + 1] = tmp;
				find = 1;
			}
			++i;
		}
	}
}
