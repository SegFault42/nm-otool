/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/23 20:43:57 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/08 02:58:01 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

static void	loop(uint64_t i, uint64_t s, struct section_64 *c)
{
	size_t	j;
	char	*itoa;

	if (i % 16 == 0 && i != s)
	{
		RC;
		itoa = ft_hexa_itoa(c->addr + i, 0);
		j = ft_strlen(itoa) - 1;
		while (++j < 16)
			ft_dprintf(1, "0");
		ft_dprintf(1, "%s	", itoa);
		free(itoa);
	}
}

static void	print_otool_64(struct section_64 *sec, char *ptr)
{
	uint64_t		i;
	uint64_t		size;
	unsigned char	*data;
	char			*itoa;
	size_t			j;

	i = 0;
	data = (unsigned char*)ptr + (sec)->offset;
	itoa = ft_hexa_itoa(sec->addr, 0);
	j = ft_strlen(itoa) - 1;
	while (++j < 16)
		ft_dprintf(1, "0");
	ft_dprintf(1, "%s	", itoa);
	free(itoa);
	size = (sec)->size;
	while (i < size)
	{
		if ((data[i] >= 0 && data[i] <= 15))
			ft_dprintf(1, "0");
		ft_dprintf(1, "%x ", data[i]);
		i++;
		loop(i, size, sec);
	}
}

static bool	print_output_64(struct load_command *lc, char *p)
{
	struct segment_command_64	*seg;
	struct section_64			*sec;
	unsigned int				i;

	i = 0;
	seg = (struct segment_command_64 *)lc;
	sec = (struct section_64 *)((char *)seg +
			sizeof(struct segment_command_64));
	if (seg->vmsize == 0)
		return (false);
	while (i < seg->nsects)
	{
		if (ft_strcmp(sec->sectname, SECT_TEXT) == 0 &&
			ft_strcmp(sec->segname, SEG_TEXT) == 0)
			print_otool_64(sec, p);
		sec++;
		i++;
	}
	return (true);
}

void		handle_64(char *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	unsigned int			i;

	i = 0;
	header = (struct mach_header_64*)ptr;
	lc = (void*)(ptr + sizeof(struct mach_header_64));
	if (header->filetype > 0xb)
	{
		printf(RED"Invalid file\n"END);
		return ;
	}
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			if (print_output_64(lc, ptr) == false)
				return ;
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
	ft_putchar('\n');
}
