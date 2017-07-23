#include "nm.h"

static void	swap(char **array, uint32_t nb)
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

static void	delete_same_value(char **array, uint32_t nb)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (i < nb)
	{
		while (!array[i])
			++i;
		j = i;
		++j;
		while (j < nb)
		{
			if ((array[j] && ft_strcmp(array[i], array[j]) == 0))
				ft_strdel(&array[j]);
			if ((array[j] && ft_strcmp(&array[i][11], &array[j][11]) == 0) &&
			ft_strncmp(array[i], "        ", 8) == 0)
			{
				ft_strdel(&array[i]);
				j = nb;
			}
			j++;
		}
		++i;
	}
	i = 0;
	while (i < nb)
	{
		if (array[i] && array[i][11] == '/')
			ft_strdel(&array[i]);
		if (array[i] && ft_strlen(array[i]) == 11)
			ft_strdel(&array[i]);
		if (array[i] && array[i][9] == '0')
			ft_strdel(&array[i]);
		++i;
	}
	swap(array, nb);
	swap(array, nb);
}

static void	print_output_32(struct symtab_command *sym, char *ptr)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist	*array;
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
		output[i] = (char *)ft_memalloc(sizeof(char) * 11 + ft_strlen(stringtable + array[i].n_un.n_strx) + 1 +2);
		if (!output)
			ft_critical_error(MALLOC_ERROR);
		if (array[i].n_value)
		{
			hexa_itoa = ft_hexa_itoa(array[i].n_value, 0);
			ft_strxcat(output[i], "0", 8 - (int)ft_strlen(hexa_itoa));
			ft_strcat(output[i], hexa_itoa);
			ft_strdel(&hexa_itoa);
		}
		else
			ft_strxcat(output[i], " ", 8);
		ft_strcat(output[i], " ");


		if ((array[i].n_type ^ array[i].n_sect) == 0x1)
			ft_strcat(output[i], "U ");
		else if ((array[i].n_type ^ array[i].n_sect) == 15 || (array[i].n_type ^ array[i].n_sect) == 37 || (array[i].n_type ^ array[i].n_sect) == 31)
			ft_strcat(output[i], "t ");
		else if ((array[i].n_type ^ array[i].n_sect) == 29)
			ft_strcat(output[i], "S ");
		else if ((array[i].n_type ^ array[i].n_sect) == 5 ||
				(array[i].n_type ^ array[i].n_sect) == 19 ||
				(array[i].n_type ^ array[i].n_sect) == 32 ||
				(array[i].n_type ^ array[i].n_sect) == 45 ||
				(array[i].n_type ^ array[i].n_sect) == 24)
			ft_strcat(output[i], "s ");
		else if ((array[i].n_type ^ array[i].n_sect) == 14)
			ft_strcat(output[i], "T ");
		else if ((array[i].n_type ^ array[i].n_sect) == 0 || (array[i].n_type ^ array[i].n_sect) == 40)
			ft_strcat(output[i], "b ");
		else
			ft_strcat(output[i], "0 ");
		ft_strcat(output[i], stringtable + array[i].n_un.n_strx); // stock le nom
		++i;
	}
	ft_sort_double_array_32(output);
	delete_same_value(output, sym->nsyms);
	ft_sort_double_array_32(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
}

void	handle_32(char *ptr)
{
	struct load_command		*lc;
	struct symtab_command	*sym;
	struct mach_header	*header;
	uint32_t				i;
	uint32_t				ncmds;

	i = 0;
	header = (void *)ptr; // header pointe sur ptr (octet 0 du binaire)
	ncmds = header->ncmds; // ncmds contient le nombre de load_command
	lc = (void *)ptr + sizeof(struct mach_header); // lc pointe sur le debut de la zone des load commands (juste apres le header)
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
			print_output_32(sym, ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
	}
}
