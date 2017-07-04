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
	t_setup	stp;
	int		i;

	i = 0;
	ft_memset(&stp, 0, sizeof(t_setup));
	if (argc == 1)
	{
		stp.ptr = setup_unset("./a.out", &stp, 's');
		nm(stp.ptr);
		if (setup_unset("a.out", &stp, 'u') == (char *)-1)
			return (EXIT_FAILURE);
	}
	else
	{
		while (++i < argc)
		{
			if (argc > 2)
				ft_dprintf(1, "\n%s:\n", argv[i]);
			stp.ptr = setup_unset(argv[i], &stp, 's');
			if (!stp.ptr)
				return (EXIT_FAILURE);
			nm(stp.ptr);
			if (setup_unset(argv[i], &stp, 'u') == (char *)-1)
				return (EXIT_FAILURE);
		}
	}
	return (0);
}
