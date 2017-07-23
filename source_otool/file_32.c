#include "otool.h"

static void	print_otool_32(struct section *sec, char *ptr)
{
	uint64_t		i;
	uint64_t		size;
	unsigned char	*data;
	char			*itoa;

	data = (unsigned char*)ptr + (sec)->offset;
	i = 0;
	itoa = ft_hexa_itoa(sec->addr, 0);
	ft_dprintf(1, "0000%s ", itoa);
	free(itoa);
	size = (sec)->size;
	while (i < size)
	{
		if ((data[i] >= 0 && data[i] <= 15))
			ft_dprintf(1, "0");
		ft_dprintf(1, "%x ", data[i]);
		i++;
		if (i % 16 == 0 && i != size)
		{
			itoa = ft_hexa_itoa(sec->addr + i, 0);
			ft_dprintf(1, "\n0000%s ", itoa);
			free(itoa);
		}
	}
}

static void	print_output_32(struct load_command *lc, uint32_t filetype, char *ptr)
{
	struct segment_command	*seg;
	struct section			*sec;
	unsigned int			i;

	i = 0;
	seg = (struct segment_command *)lc;
	sec = (struct section *)((char *)seg + sizeof(struct segment_command));
	while (i < seg->nsects)
	{
		if (ft_strcmp(sec->sectname, SECT_TEXT) == 0 &&
			ft_strcmp(sec->segname, SEG_TEXT) == 0)
		{
			if (filetype == MH_EXECUTE)
				print_otool_32(sec, ptr);
		}
		sec++;
		i++;
	}
}

void	handle_32(char *ptr)
{
	struct mach_header	*header;
	struct load_command	*lc;
	unsigned int		i;

	i = 0;
	header = (struct mach_header *)ptr;
	lc = (void*)(ptr + sizeof(struct mach_header));
	if (header->filetype > 0xb)
	{
		printf(RED"Invalid file\n"END);
		return ;
	}
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
			print_output_32(lc, header->filetype, ptr);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
	ft_putchar('\n');
}
