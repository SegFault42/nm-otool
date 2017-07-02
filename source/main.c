#include "nm.h"

void	print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int	i;
	char	*stringtable;
	struct nlist_64 *array;

	i = 0;
	array = (void *)ptr + symoff;
	stringtable = ptr + stroff;
	while (i < nsyms)
	{
		printf("%s\n", stringtable + array[i].n_un.n_strx);
		++i;
	}
}

static void	handle_64(char *ptr)
{
	struct load_command		*lc;
	struct mach_header_64	*header;
	struct symtab_command	*sym;
	uint32_t				ncmds;
	uint32_t				i;

	i = 0;
	header = (void *)ptr;
	ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(struct mach_header_64);
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (void*)lc + lc->cmdsize;
		++i;
	}

}

static void	nm(char *ptr)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t *)(void *)ptr;

	if (magic_number == MH_MAGIC_64)
		handle_64(ptr);
}

int	main(int argc, char **argv)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if (argc != 2)
	{
		ft_dprintf(STDERR_FILENO, "One arg\n");
		return (EXIT_FAILURE);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "Open argv[1] failure\n");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buf) == -1)
	{
		ft_dprintf(STDERR_FILENO, "fstat failure\n");
		return (EXIT_FAILURE);
	}
	ptr = mmap(0, (size_t)buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
	{
		ft_dprintf(STDERR_FILENO, "mmap failure\n");
		return (EXIT_FAILURE);
	}
	nm(ptr);
	// code goes here ...
	if (munmap(ptr, (size_t)buf.st_size) < 0)
	{
		ft_dprintf(STDERR_FILENO, "munap failure\n");
		return (EXIT_FAILURE);
	}
	return (0);
}
