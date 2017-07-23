#include "nm.h"

static void	swap_64(char **array, uint32_t nb)
{
	uint32_t	i;
	uint32_t	j;

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
		}
		++i;
	}
}

static void	delete_same_value_64(char **array, uint32_t nb)
{
	uint32_t	i;

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
	swap_64(array, nb);
}

static void	print_output_64(struct symtab_command *sym, struct mach_header_64 *header, char *ptr)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;
	char			**output;
	char			*hexa_itoa;
	size_t			flag;
	size_t			flag_xor;
	size_t			flag_and;
	size_t			flag_or;

	i = 0;
	array = (void *)ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;
	output = (char **)malloc(sizeof(char *) * sym->nsyms + 1);
	output[sym->nsyms] = NULL;
	if (!output)
		ft_critical_error(MALLOC_ERROR);
	while (i < sym->nsyms)
	{
		flag_and = (array[i].n_type & array[i].n_sect);
		flag_or = (array[i].n_type | array[i].n_sect);
		flag_xor = (array[i].n_type ^ array[i].n_sect);
		flag = (array[i].n_type & array[i].n_sect) + (array[i].n_type | array[i].n_sect) + (array[i].n_type ^ array[i].n_sect);
		output[i] = (char *)ft_memalloc(sizeof(char) * 19 + ft_strlen(stringtable + array[i].n_un.n_strx) + 1 +29);
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
		if (header->filetype == MH_EXECUTE)
		{
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
		}
		else if (header->filetype == MH_OBJECT)
		{
			if ((flag_xor) == 0x1)
				ft_strcat(output[i], "U ");
			else if ((flag_xor) == 15)
				ft_strcat(output[i], "t ");
			else if ((flag_xor) == 14)
			{
				if (ft_strncmp(output[i], "                ", 16) == 0)
					ft_memset(output[i], '0', 16);
				ft_strcat(output[i], "T ");
			}
			else if ((flag_xor) == 7)
				ft_strcat(output[i], "S ");
			else
				ft_strcat(output[i], "0 ");
		}
		else if (header->filetype == MH_BUNDLE)
		{
			if ((flag_xor) == 14)
				ft_strcat(output[i], "T ");
			else if ((flag_xor) == 1)
				ft_strcat(output[i], "U ");
			else if ((flag_xor) == 0 ||
					(flag_xor) == 22 ||
					(flag_xor) == 21 ||
					(flag_xor) == 26)
				ft_strcat(output[i], "S ");
			else
				ft_strcat(output[i], "0 ");
		}
		else if (header->filetype == MH_DYLIB)
		{
			if (flag == 62 || flag == 30)
				ft_strcat(output[i], "t ");
			else if (flag_xor == 26 && flag_or == 30 && flag_and == 4)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 10 && flag_or == 14 && flag_and == 4)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 4 && flag_or == 14 && flag_and == 10)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 4 && flag_or == 30 && flag_and == 26)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 30 && flag_or == 30 && flag_and == 0)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 14 && flag_or == 30 && flag_and == 16)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 11 && flag_or == 15 && flag_and == 4)
				ft_strcat(output[i], "S ");
			else if (flag_xor == 19 && flag_or == 31 && flag_and == 12)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 2 && flag_or == 30 && flag_and == 28)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 24 && flag_or == 30 && flag_and == 6)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 8 && flag_or == 14 && flag_and == 6)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 11 && flag_or == 15 && flag_and == 14)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 15 && flag_or == 15 && flag_and == 0)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 3 && flag_or == 15 && flag_and == 12)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 7 && flag_or == 31 && flag_and == 24)
				ft_strcat(output[i], "s ");
			else if (flag_xor == 9 && flag_or == 15 && flag_and == 6)
				ft_strcat(output[i], "S ");
			else if (flag_xor == 2 && flag_or == 15 && flag_and == 13)
				ft_strcat(output[i], "S ");
			else if (flag_xor == 24 && flag_or == 31 && flag_and == 7)
				ft_strcat(output[i], "S ");
			else if (flag_xor == 25 && flag_or == 31 && flag_and == 6)
				ft_strcat(output[i], "S ");
			else if (flag_xor == 4 && flag_or == 15 && flag_and == 11)
				ft_strcat(output[i], "S ");
			else if (flag_xor == 5 && flag_or == 15 && flag_and == 10)
				ft_strcat(output[i], "S ");
			else if (flag_xor == 10 && flag_or == 15 && flag_and == 5)
				ft_strcat(output[i], "S ");
			else if (flag_xor == 14 && flag_or == 15 && flag_and == 1)
				ft_strcat(output[i], "T ");
			else if (flag_xor == 1 && flag_or == 1 && flag_and == 0)
				ft_strcat(output[i], "U ");
			else if (flag_xor == 1 && flag_or == 15 && flag_and == 14 && array[i].n_desc == 128)
				ft_strcat(output[i], "D ");
			else if (flag_xor == 23 && flag_or == 31 && flag_and == 8)
				ft_strcat(output[i], "D ");
			else if (flag_xor == 1 && flag_or == 15 && flag_and == 14 && array[i].n_desc == 0)
				ft_strcat(output[i], "b ");
			else if (flag_xor == 2 && flag_or == 14 && flag_and == 12)
				ft_strcat(output[i], "b ");
			else if (flag_xor == 18 && flag_or == 30 && flag_and == 12)
				ft_strcat(output[i], "b ");
			else if (flag_xor == 20 && flag_or == 30 && flag_and == 10)
				ft_strcat(output[i], "b ");
			else if (flag_xor == 8 && flag_or == 14 && flag_and == 6)
				ft_strcat(output[i], "d ");
			else if (flag_xor == 22 && flag_or == 30 && flag_and == 8)
				ft_strcat(output[i], "d ");
			else if (flag_xor == 16 && flag_or == 30 && flag_and == 14)
				ft_strcat(output[i], "d ");
			else if (flag_xor == 6 && flag_or == 30 && flag_and == 24)
				ft_strcat(output[i], "d ");
			else if (flag_xor == 21 && flag_or == 31 && flag_and == 10)
				ft_strcat(output[i], "d ");
			else if (flag_xor == 0 && flag_or == 14 && flag_and == 14)
				ft_strcat(output[i], "d ");
			else
				ft_strcat(output[i], "0 ");
		}
		ft_strcat(output[i], stringtable + array[i].n_un.n_strx); // stock le nom

		/*printf("%s ", output[i]);*/
		/*printf("n_type = %d | ", array[i].n_type);*/
		/*printf("n_sect = %d |", array[i].n_sect); // debug print for symbole*/
		/*printf("n_desc = %d |", array[i].n_desc);*/
		/*printf("n_value = %lld\n", array[i].n_value);*/
		/*printf("%d\n", array[i].n_type & array[i].n_sect);*/

		/*ft_strcat(output[i], " ");*/
		/*ft_strcat(output[i], ft_itoa((int)flag_xor)); // debug flag NE PAS OUBLIER DE SUPPRIMER LE MALLOC + 2*/
		/*ft_strcat(output[i], " ");*/
		/*ft_strcat(output[i], ft_itoa((int)flag_or)); // debug flag NE PAS OUBLIER DE SUPPRIMER LE MALLOC + 2*/
		/*ft_strcat(output[i], " ");*/
		/*ft_strcat(output[i], ft_itoa((int)flag_and)); // debug flag NE PAS OUBLIER DE SUPPRIMER LE MALLOC + 2*/
		/*ft_strcat(output[i], " ");*/
		/*ft_strcat(output[i], ft_itoa((int)array[i].n_desc)); // debug flag NE PAS OUBLIER DE SUPPRIMER LE MALLOC + 2*/
		++i;
	}
	ft_sort_double_array(output);
	delete_same_value_64(output, sym->nsyms);
	ft_sort_double_array(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
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
	if (header->filetype > 0xb)
	{
		printf(RED"Invalid file\n"END);
		return ;
	}
	while (i < ncmds) // on iter autant de fois qu'il y a de load commands
	{
		if (lc->cmd == LC_SYMTAB) // si la cmd est egal a LC_SYMTAB
		{
			sym = (struct symtab_command *)lc;
			print_output_64(sym, header, ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
	}
}
