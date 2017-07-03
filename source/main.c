#include "nm.h"

static void	print_output(uint32_t nsyms, uint32_t symoff, uint32_t stroff, char *ptr)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64 *array;

	i = 0;
	array = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;
	while (i < nsyms)
	{
		if (array[i].n_value)
			printf("%.16llx ", array[i].n_value);
		else
			printf("                 ");
		if (array[i].n_type == N_TYPE)
			printf("t ");
		else if (array[i].n_type == N_EXT)
			printf("U ");
		else if (array[i].n_type == 0xf)
			printf("T ");
		printf("%s\n", stringtable + array[i].n_un.n_strx);
		++i;
	}
}

static void	handle_64(char *ptr)
{
	struct load_command		*lc;
	struct symtab_command	*sym;
	struct mach_header_64	*header;
	uint32_t				i;
	uint32_t				ncmds;

	i = 0;
	header = (void *)ptr; // header pointe sur ptr (octet 0 du binaire)
	ncmds = header->ncmds; // ncmds contient le nombre de load_command
	lc = (void *)ptr + sizeof(struct mach_header_64); // lc pointe sur le debut de la zone des load commands (juste apres le header)
	while (i < ncmds) // on iter autant de fois qu'il y a de load commands
	{
		if (lc->cmd == LC_SYMTAB) // si la cmd est egal a LC_SYMTAB
		{
			sym = (struct symtab_command *)lc;
			printf("cmdsize =					%d\n", sym->cmdsize);
			printf("symbol tab offset =				%d\n", sym->symoff);
			printf("number of symbol table entries =		%d\n", sym->nsyms);
			printf("string table offset =				%d\n", sym->stroff);
			printf("string table size in byte =			%d\n", sym->strsize);
			RC;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
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
	if (munmap(ptr, (size_t)buf.st_size) < 0)
	{
		ft_dprintf(STDERR_FILENO, "munap failure\n");
		return (EXIT_FAILURE);
	}
	return (0);
}
