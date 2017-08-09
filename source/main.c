/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/08 03:46:07 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/09 04:00:06 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int	reverse_int(int x)
{
	x = ((x << 8) & 0xFF00FF00) | ((x >> 8) & 0xFF00FF);
	return (x << 16) | (x >> 16);
}

void		nm(char *ptr)
{
	uint32_t	magic_number;
	static int	i = 0;

	magic_number = *(uint32_t *)(void *)ptr;
	if (magic_number == MH_MAGIC_64)
		handle_64(ptr);
	else if (magic_number == MH_MAGIC)
		handle_32(ptr);
	else if (i == 0 && (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM))
	{
		++i;
		fat_handle(ptr);
	}
	else
		ft_dprintf(2, RED"NOT SUPPORTED YET :(\n"END);
}

void		fat_handle(char *file)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	int					i;
	int					offset;

	i = 0;
	offset = 0;
	header = (struct fat_header *)file;
	arch = (void*)&header[1];
	while (i < reverse_int(header->nfat_arch))
	{
		if (reverse_int(arch->cputype) == CPU_TYPE_X86_64 ||
				reverse_int(arch->cputype) == CPU_TYPE_X86)
			offset = arch->offset;
		++i;
		++arch;
	}
	nm(file + reverse_int(offset));
}

bool		argc_equal_1(t_setup *setup, int argc)
{
	setup->ptr = setup_unset(argc, "./a.out", setup, 's');
	if (!setup->ptr)
		return (false);
	nm(setup->ptr);
	if (setup_unset(argc, "a.out", setup, 'u') == (char *)-1)
		return (false);
	return (true);
}

int			main(int argc, char **argv)
{
	t_setup	setup;
	int		i;

	i = 0;
	ft_memset(&setup, 0, sizeof(t_setup));
	if (argc == 1)
	{
		if (argc_equal_1(&setup, argc) == false)
			return (EXIT_FAILURE);
	}
	else
	{
		while (++i < argc)
		{
			setup.ptr = setup_unset(argc, argv[i], &setup, 's');
			if (!setup.ptr)
				continue ;
			nm(setup.ptr);
			if (setup_unset(argc, argv[i], &setup, 'u') == (char *)-1)
				return (EXIT_FAILURE);
		}
	}
	return (0);
}
