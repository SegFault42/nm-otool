/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/23 20:39:17 by rabougue          #+#    #+#             */
/*   Updated: 2017/07/23 20:40:04 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

static void	o_tool(char *ptr, char *argv)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t *)(void *)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		ft_dprintf(1, "%s:\n(__TEXT,__text) section\n", argv);
		handle_64(ptr);
	}
	else if (magic_number == MH_MAGIC)
	{
		ft_dprintf(1, "%s:\n(__TEXT,__text) section\n", argv);
		handle_32(ptr);
	}
	else
		ft_dprintf(2, RED"%s File not supported\n"END, argv);
}

int	main(int argc, char **argv)
{
	t_setup	setup;
	int		i;

	i = 0;
	ft_memset(&setup, 0, sizeof(t_setup));
	while (++i < argc)
	{
		setup.ptr = setup_unset(argv[i], &setup, 's');
		if (!setup.ptr)
			continue ;
		o_tool(setup.ptr, argv[i]);
		if (setup_unset(argv[i], &setup, 'u') == (char *)-1)
			return (EXIT_FAILURE);
	}
	return (0);
}
