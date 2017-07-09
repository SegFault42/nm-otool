#include "nm.h"

static void	swap_64(char **array, int nb)
{
	int i;
	int j;

	i = 0;
	j = nb;
	while (i < j)
	{
		if (array[i] == NULL)
		{
			while (array[j] == NULL)
				j--;
			array[i] = ft_strdup(array[j]);
			ft_strdel(&array[j]);
			/*array[j] = NULL;*/
		}
		++i;
	}
	/*for (int a = 0; a < nb; ++a)*/
		/*printf("%s\n", array[a]);*/
	/*ft_print_2d_tab(array);*/
	/*sleep(10);*/
}

static void	delete_same_value_64(char **array, int nb)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	while (i < nb)
	{
		while (array[i] == NULL)
			++i;
		j = i;
		++j;
		while (j < nb)
		{
			if (array[j] == NULL)
				++j;
			if (&array[j][19] && &array[i][19] && ft_strcmp(&array[i][19], &array[j][19]) == 0)
				ft_strdel(&array[j]);
			++j;
		}
		++i;
	}
	i = 0;
	while (i < nb)
	{
		if (array[i] && array[i][19] == '/')
			ft_strdel(&array[i]);
		if (array[i] && ft_strlen(array[i]) == 19)
			ft_strdel(&array[i]);
		if (array[i] && array[i][17] == '0')
			ft_strdel(&array[i]);
		++i;
	}
	swap_64(array, nb);
}

static void	print_output_64_obj(struct symtab_command *sym, char *ptr)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;
	char			**output;
	char			*hexa_itoa;

	i = 0;
	array = (void *)ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;
	output = (char **)malloc(sizeof(char *) * sym->nsyms + 1);
	output[sym->nsyms] = NULL;
	if (!output)
		ft_critical_error(MALLOC_ERROR);
	while (i < sym->nsyms)
	{
		output[i] = (char *)ft_memalloc(sizeof(char) * 19 + ft_strlen(stringtable + array[i].n_un.n_strx) + 1 +2);
		if (!output)
			ft_critical_error(MALLOC_ERROR);
		if (array[i].n_value)
		{
			hexa_itoa = ft_hexa_itoa(array[i].n_value, 0);
			ft_strxcat(output[i], "0", 16 - (int)ft_strlen(hexa_itoa));
			ft_strcat(output[i], hexa_itoa);
			ft_strdel(&hexa_itoa);
		}
		else
			ft_strxcat(output[i], " ", 16);
		ft_strcat(output[i], " ");

		/*printf("%s ", output[i]);*/
		/*printf("n_type = %d | ", array[i].n_type);*/
		/*printf("n_sect = %d |", array[i].n_sect); // debug print for symbole*/
		/*printf("n_desc = %d |", array[i].n_desc);*/
		/*printf("n_value = %lld ", array[i].n_value);*/
		/*printf("%s\n", stringtable + array[i].n_un.n_strx);*/
		/*printf("%d\n", array[i].n_type | array[i].n_sect);*/

		if ((array[i].n_type | array[i].n_sect) == 0x1)
			ft_strcat(output[i], "U ");
		else if ((array[i].n_type | array[i].n_sect) == 15 && ft_strncmp(output[i], "                ", 16) == 0)
		{
			ft_memset(output[i], '0', 16);
			ft_strcat(output[i], "T ");
		}
		else if ((array[i].n_type | array[i].n_sect) == 15)
			ft_strcat(output[i], "t ");
		else
			ft_strcat(output[i], "0 ");
		ft_strcat(output[i], stringtable + array[i].n_un.n_strx); // stock le nom
		/*ft_strcat(output[i], " ");*/
		/*ft_strcat(output[i], ft_itoa(array[i].n_type)); // debug flag*/
		++i;
	}
	ft_sort_double_array(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
}

static void	print_output_64(struct symtab_command *sym, char *ptr)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;
	char			**output;
	char			*hexa_itoa;

	i = 0;
	array = (void *)ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;
	output = (char **)malloc(sizeof(char *) * sym->nsyms + 1);
	output[sym->nsyms] = NULL;
	if (!output)
		ft_critical_error(MALLOC_ERROR);
	while (i < sym->nsyms)
	{
		output[i] = (char *)ft_memalloc(sizeof(char) * 19 + ft_strlen(stringtable + array[i].n_un.n_strx) + 1 +2);
		if (!output)
			ft_critical_error(MALLOC_ERROR);
		if (array[i].n_value)
		{
			hexa_itoa = ft_hexa_itoa(array[i].n_value, 0);
			ft_strxcat(output[i], "0", 16 - (int)ft_strlen(hexa_itoa));
			ft_strcat(output[i], hexa_itoa);
			ft_strdel(&hexa_itoa);
		}
		else
			ft_strxcat(output[i], " ", 16);
		ft_strcat(output[i], " ");

		if ((array[i].n_type | array[i].n_sect) == 0x1)
			ft_strcat(output[i], "U ");
		else if ((array[i].n_type | array[i].n_sect) == 37)
			ft_strcat(output[i], "t ");
		else if ((array[i].n_type | array[i].n_sect) == 15)
			ft_strcat(output[i], "T ");
		else if ((array[i].n_type & array[i].n_sect) == 0x8)
			ft_strcat(output[i], "d ");
		else if ((array[i].n_type & array[i].n_sect) == 0x9 || (array[i].n_type & array[i].n_sect) == 0xc)
			ft_strcat(output[i], "D ");
		else if ((array[i].n_type & array[i].n_sect) == 0xb || (array[i].n_type & array[i].n_sect) == 0xd)
			ft_strcat(output[i], "S ");
		else if ((array[i].n_type & array[i].n_sect) == 0xa)
			ft_strcat(output[i], "b ");
		else
			ft_strcat(output[i], "0 ");
		ft_strcat(output[i], stringtable + array[i].n_un.n_strx); // stock le nom
		/*ft_strcat(output[i], " ");*/
		/*ft_strcat(output[i], ft_itoa(array[i].n_type | array[i].n_sect)); // debug flag*/
		/*printf("%s ", output[i]);*/
		/*printf("n_type = %d | ", array[i].n_type);*/
		/*printf("n_sect = %d |", array[i].n_sect); // debug print for symbole*/
		/*printf("n_desc = %d |", array[i].n_desc);*/
		/*printf("n_value = %lld ", array[i].n_value);*/
		/*printf("%s\n", stringtable + array[i].n_un.n_strx);*/
		/*printf("%d\n", array[i].n_type | array[i].n_sect);*/
		++i;
	}
	ft_sort_double_array(output);
	delete_same_value_64(output, sym->nsyms);
	ft_sort_double_array(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
}

void	handle_64_revers(char *ptr)
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
			print_output_64(sym, ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
	}
}

void	handle_64(char *ptr)
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
			if (header->filetype == MH_OBJECT)
				print_output_64_obj(sym, ptr);
			else
				print_output_64(sym, ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
	}
}
