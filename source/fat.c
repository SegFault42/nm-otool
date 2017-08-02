#include "../include/nm.h"

static int	reverse_int(int x)
{
	x = ((x << 8) & 0xFF00FF00) | ((x >> 8) & 0xFF00FF);
	return (x << 16) | (x >> 16);
}

void	fat_handle(char *file, char *str)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	int					i;
	int					offset;

	i = 0;
	header = (struct fat_header *)file;
	arch = (void*)&header[1];
	while (i < reverse_int(header->nfat_arch))
	{
		if (reverse_int(arch->cputype) == CPU_TYPE_X86_64
				|| reverse_int(arch->cputype) == CPU_TYPE_X86)
			offset = arch->offset;
		++i;
		++arch;
	}
	nm(file + reverse_int(offset), str);
}
