#include "nm.h"

static void	print_output(uint32_t nsyms, uint32_t symoff, uint32_t stroff, char *ptr)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;
	char			**output;
	char			*hexa_itoa;

	i = 0;
	array = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;
	output = (char **)malloc(sizeof(char *) * nsyms + 1);
	output[nsyms] = NULL;
	if (!output)
		ft_critical_error(MALLOC_ERROR);
	while (i < nsyms)
	{
		output[i] = (char *)ft_memalloc(sizeof(char) * 19 + ft_strlen(stringtable + array[i].n_un.n_strx) + 1);
		if (!output)
			ft_critical_error(MALLOC_ERROR);
		if (array[i].n_value)
		{
			hexa_itoa = ft_hexa_itoa(array[i].n_value, 0);
			ft_strxcat(output[i], "0", 16 - ft_strlen(hexa_itoa));
			ft_strcat(output[i], hexa_itoa);
			ft_strdel(&hexa_itoa);
		}
		else
			ft_strxcat(output[i], " ", 16);
		ft_strcat(output[i], " ");
		if (array[i].n_type == N_TYPE)
			ft_strcat(output[i], "t ");
		else if (array[i].n_type == N_EXT)
			ft_strcat(output[i], "U ");
		else if (array[i].n_type == 0xf)
			ft_strcat(output[i], "T ");
		ft_strcat(output[i], stringtable + array[i].n_un.n_strx);
		++i;
	}
	ft_sort_double_array(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
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
			/*printf("cmdsize =					%d\n", sym->cmdsize);*/
			/*printf("symbol tab offset =				%d\n", sym->symoff);*/
			/*printf("number of symbol table entries =		%d\n", sym->nsyms);*/
			/*printf("string table offset =				%d\n", sym->stroff);*/
			/*printf("string table size in byte =			%d\n", sym->strsize);*/
			/*RC;*/
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
	t_setup	setup;
	int		i;

	i = 0;
	ft_memset(&setup, 0, sizeof(t_setup));
	if (argc == 1)
	{
		setup.ptr = setup_unset("./a.out", &setup, 's');
		nm(setup.ptr);
		if (setup_unset("a.out", &setup, 'u') == (char *)-1)
			return (EXIT_FAILURE);
	}
	else
	{
		while (++i < argc)
		{
			if (argc > 2)
				ft_dprintf(1, "\n%s:\n", argv[i]);
			setup.ptr = setup_unset(argv[i], &setup, 's');
			if (!setup.ptr)
				return (EXIT_FAILURE);
			nm(setup.ptr);
			if (setup_unset(argv[i], &setup, 'u') == (char *)-1)
				return (EXIT_FAILURE);
		}
	}
	return (0);
}
