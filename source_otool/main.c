/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/23 20:39:17 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/08 02:59:52 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

static uint32_t	reverse_int(uint32_t x)
{
	x = ((x << 8) & 0xFF00FF00) | ((x >> 8) & 0xFF00FF);
	return (x << 16) | (x >> 16);
}

static void	o_tool(char *ptr, char *argv)
{
	uint32_t	magic_number;
	static int	i = 0;

	magic_number = *(uint32_t *)(void *)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		ft_dprintf(1, "%s:\nContents of (__TEXT,__text) section\n", argv);
		handle_64(ptr);
	}
	else if (magic_number == MH_MAGIC)
	{
		ft_dprintf(1, "%s:\nContents of (__TEXT,__text) section\n", argv);
		handle_32(ptr);
	}
	else if (i == 0 && (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM))
	{
		++i;
		fat_handle(ptr, argv);
	}
	else
		ft_dprintf(2, RED"%s File not supported\n"END, argv);
}

void		fat_handle(char *file, char *argv)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	uint32_t			i;
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
	o_tool(file + reverse_int(offset), argv);
}

int			main(int argc, char **argv)
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
