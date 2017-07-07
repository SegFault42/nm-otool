#include "nm.h"


static void	print_output_64(uint32_t nsyms, uint32_t symoff, uint32_t stroff, char *ptr)
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
		/*printf("n_value = %lld\n", array[i].n_value);*/
		/*printf("%d\n", array[i].n_type & array[i].n_sect);*/

		if ((array[i].n_type & array[i].n_sect) == 0x0)
			ft_strcat(output[i], "U ");
		else if ((array[i].n_type & array[i].n_sect) == 0x1)
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
		/*ft_strcat(output[i], ft_itoa(array[i].n_type)); // debug flag*/
		++i;
	}
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
			print_output_64(sym->nsyms, sym->symoff, sym->stroff, ptr);
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
			print_output_64(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
	}
}
