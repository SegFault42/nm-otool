#include "nm.h"

void	print_otool_64(struct section_64 *sec, char *ptr)
{
	uint64_t		i;
	uint64_t		size;
	unsigned char	*data;
	char			*itoa;

	data = (unsigned char*)ptr + (sec)->offset;
	i = 0;
	/*ft_dprintf(1, "%x\t", (sec)->addr);*/
	/*ft_print_hexa((unsigned long long)sec->addr);*/
	itoa = ft_hexa_itoa(sec->addr, 0);
	ft_dprintf(1, "0000000%s ", itoa);
	free(itoa);
	size = (sec)->size;
	while (i < size)
	{
		if ((data[i] >= 0 && data[i] <= 15)/* || (data[i] >= 11 && data[i] <= 16)*/)
			ft_dprintf(1, "0");
		ft_dprintf(1, "%x ", data[i]);
		i++;
		if (i % 16 == 0 && i != size)
		{
			itoa = ft_hexa_itoa(sec->addr + i, 0);
			ft_dprintf(1, "\n0000000%s ", itoa);
			free(itoa);
		}
	}
}

void	print_output_64(struct load_command *lc, uint32_t filetype, char *ptr)
{
	struct segment_command_64	*seg;
	struct section_64			*sec;
	unsigned int				i;

	i = 0;
	seg = (struct segment_command_64*)lc;
	sec = (struct section_64 *)((char *)seg + sizeof(struct segment_command_64));
	while (i < seg->nsects)
	{
		if (ft_strcmp(sec->sectname, SECT_TEXT) == 0 && ft_strcmp(sec->segname, SEG_TEXT) == 0)
		{
			if (filetype == MH_EXECUTE)
				print_otool_64(sec, ptr);
		}
		sec++;
		i++;
	}
}

void	handle_64(char *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	unsigned int			i;

	i = 0;
	header = (struct mach_header_64*)ptr;
	lc = (void*)(ptr + sizeof(struct mach_header_64));
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			print_output_64(lc, header->filetype, ptr);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
	ft_putchar('\n');
}
