/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 20:26:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/07/29 23:31:41 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static char	use_buf(char *str)
{
	if (!ft_strcmp(str, SECT_TEXT))
		return ('T');
	if (!ft_strcmp(str, SECT_DATA))
		return ('D');
	if (!ft_strcmp(str, SECT_BSS))
		return ('B');
	return ('S');
}

static char	symbol(size_t n_type, int value, char *buf)
{
	size_t	tmp;
	char	r;

	tmp = n_type;
	r = 0;
	n_type = n_type & N_TYPE;
	if (n_type == N_UNDF || n_type == N_PBUD)
	{
		if (value)
			r = 'C';
		else
			r = 'U';
	}
	else if (n_type == N_ABS)
		r = 'A';
	else if (n_type == N_INDR)
		r = 'I';
	else if (n_type == N_SECT)
		r = use_buf(buf);
	if (!(tmp & N_EXT) && r)
		r += 32;
	return (r);
}


static char	*get_symbol(struct symtab_command *sym, char *ptr, char **array)
{
	struct nlist_64		*tab;
	size_t				index;
	char				*type;

	index = 0;
	tab = (void *)ptr + sym->symoff;
	type = (char *)malloc(sizeof (char) * sym->nsyms);
	ft_memset(type, 0, sym->nsyms);
	while (index < sym->nsyms)
	{
		type[index] = symbol(tab[index].n_type, tab[index].n_value, array[tab[index].n_sect - 1]);
		++index;
	}
	/*index = 0;*/
	/*while (index < sym->nsyms)*/
	/*{*/
		/*ft_dprintf(1, "symbol = %c\n", type[index]);*/
		/*++index;*/
	/*}*/
	/*exit(-1);*/
	return (type);
}

static void	get_seg_name_64(struct segment_command_64 *segment, char **array)
{
	struct section_64	*sec;
	size_t				iter_array;
	size_t				iter_sec;

	iter_array = 0;
	iter_sec = 0;
	sec = (void *)((char *)segment + sizeof(struct segment_command_64));
	while (array[iter_array])
		++iter_array;
	while (iter_sec < segment->nsects)
	{
		array[iter_array] = sec[iter_sec].sectname;
		++iter_array;
		++iter_sec;
	}
	array[iter_array] = NULL;
}

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
	i = 0;
	while (array[i])
		++i;
	if (!array[i] && array[i + 1] && (i + 1) <= nb)
	{
		array[i] = ft_strdup(array[i + 1]);
		ft_strdel(&array[i + 1]);
	}
}

static void	free_same(char **array, uint32_t nb)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	j = 1;
	while (i < nb)
	{
		if (array[i] && array[i][17] == '0')
			ft_strdel(&array[i]);
		if (!array[i])
		{
			++i;
			continue ;
		}
		j = i;
		++j;
		while (j < nb)
		{
			if (array[i] && array[j] && ft_strequ(&array[i][19], &array[j][19]) == 1)
			{
				if (array[i][0] != ' ' && array[j][0] == ' ')
					ft_strdel(&array[j]);
				else if (array[i][0] == ' ' && array[j][0] != ' ')
				{
					ft_strdel(&array[i]);
					continue ;
				}
				else
					ft_strdel(&array[j]);
			}
			++j;
		}
		++i;
	}
}

static void	delete_same_value_64(char **array, uint32_t nb)
{
	uint32_t	i;

	i = 0;
	free_same(array, nb);
	while (i < nb)
	{
		if (array[i] && array[i][19] == '/')
			ft_strdel(&array[i]);
		if (array[i] && ft_strlen(array[i]) == 19)
			ft_strdel(&array[i]);
		if (array[i] && array[i][19] != '_' && ft_strequ(&array[i][19], "dyld_stub_binder") == 0)
			ft_strdel(&array[i]);
		++i;
	}
	swap_64(array, nb);
}

static void	print_output_64(struct symtab_command *sym, struct mach_header_64 *header, char *ptr, char **segment_name)
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
	char			*symbol;
	char			test[2];

	i = 0;
	array = (void *)ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;
	output = (char **)malloc(sizeof(char *) * sym->nsyms + 1);
	if (!output)
		ft_critical_error(MALLOC_ERROR);
	output[sym->nsyms] = NULL;
	symbol = get_symbol(sym, ptr, segment_name);
	while (i < sym->nsyms)
	{
		flag_and = (array[i].n_type & array[i].n_sect);
		flag_or = (array[i].n_type | array[i].n_sect);
		flag_xor = (array[i].n_type ^ array[i].n_sect);
		flag = (array[i].n_type & array[i].n_sect) + (array[i].n_type | array[i].n_sect) + (array[i].n_type ^ array[i].n_sect);
		output[i] = (char *)ft_memalloc(sizeof(char) * 19 + ft_strlen(stringtable + array[i].n_un.n_strx) + 1 + 29);
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
		/*if (header->filetype == MH_EXECUTE)*/
		/*{*/
			test[0] = symbol[i];
			test[1] = 0;
			ft_strcat(output[i], test);
			ft_strcat(output[i], " ");
			/*if ((array[i].n_type & array[i].n_sect) == 0x0)*/
				/*ft_strcat(output[i], "U ");*/
			/*else if ((array[i].n_type & array[i].n_sect) == 0x1)*/
				/*ft_strcat(output[i], "T ");*/
			/*else if ((array[i].n_type & array[i].n_sect) == 0x8)*/
				/*ft_strcat(output[i], "d ");*/
			/*else if ((array[i].n_type & array[i].n_sect) == 0x9 || (array[i].n_type & array[i].n_sect) == 0xc)*/
				/*ft_strcat(output[i], "D ");*/
			/*else if ((array[i].n_type & array[i].n_sect) == 0xb || (array[i].n_type & array[i].n_sect) == 0xd)*/
				/*ft_strcat(output[i], "S ");*/
			/*else if ((array[i].n_type & array[i].n_sect) == 0xa)*/
				/*ft_strcat(output[i], "b ");*/
			/*else*/
				/*ft_strcat(output[i], "0 ");*/
		/*}*/
		/*else if (header->filetype == MH_OBJECT)*/
		/*{*/
			/*if ((flag_xor) == 0x1)*/
				/*ft_strcat(output[i], "U ");*/
			/*else if ((flag_xor) == 15)*/
				/*ft_strcat(output[i], "t ");*/
			/*else if ((flag_xor) == 14)*/
			/*{*/
				/*if (ft_strncmp(output[i], "                ", 16) == 0)*/
					/*ft_memset(output[i], '0', 16);*/
				/*ft_strcat(output[i], "T ");*/
			/*}*/
			/*else if ((flag_xor) == 7)*/
				/*ft_strcat(output[i], "S ");*/
			/*else*/
				/*ft_strcat(output[i], "0 ");*/
		/*}*/
		/*else if (header->filetype == MH_BUNDLE)*/
		/*{*/
			/*if ((flag_xor) == 14)*/
				/*ft_strcat(output[i], "T ");*/
			/*else if ((flag_xor) == 1)*/
				/*ft_strcat(output[i], "U ");*/
			/*else if ((flag_xor) == 0 ||*/
					/*(flag_xor) == 22 ||*/
					/*(flag_xor) == 21 ||*/
					/*(flag_xor) == 26)*/
				/*ft_strcat(output[i], "S ");*/
			/*else*/
				/*ft_strcat(output[i], "0 ");*/
		/*}*/
		/*else if (header->filetype == MH_DYLIB)*/
		/*{*/
			/*if (flag_xor == 15 && flag_or == 15 && flag_and == 0)*/
				/*ft_strcat(output[i], "t ");*/
			/*else if (flag_xor == 26 && flag_or == 30 && flag_and == 4)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 10 && flag_or == 14 && flag_and == 4)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 4 && flag_or == 14 && flag_and == 10)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 4 && flag_or == 30 && flag_and == 26)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 30 && flag_or == 30 && flag_and == 0)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 14 && flag_or == 30 && flag_and == 16)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 11 && flag_or == 15 && flag_and == 4)*/
				/*ft_strcat(output[i], "S ");*/
			/*else if (flag_xor == 19 && flag_or == 31 && flag_and == 12)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 2 && flag_or == 30 && flag_and == 28)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 24 && flag_or == 30 && flag_and == 6)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 8 && flag_or == 14 && flag_and == 6)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 11 && flag_or == 15 && flag_and == 14)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 15 && flag_or == 15 && flag_and == 0)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 3 && flag_or == 15 && flag_and == 12)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 7 && flag_or == 31 && flag_and == 24)*/
				/*ft_strcat(output[i], "s ");*/
			/*else if (flag_xor == 9 && flag_or == 15 && flag_and == 6)*/
				/*ft_strcat(output[i], "S ");*/
			/*else if (flag_xor == 2 && flag_or == 15 && flag_and == 13)*/
				/*ft_strcat(output[i], "S ");*/
			/*else if (flag_xor == 24 && flag_or == 31 && flag_and == 7)*/
				/*ft_strcat(output[i], "S ");*/
			/*else if (flag_xor == 25 && flag_or == 31 && flag_and == 6)*/
				/*ft_strcat(output[i], "S ");*/
			/*else if (flag_xor == 4 && flag_or == 15 && flag_and == 11)*/
				/*ft_strcat(output[i], "S ");*/
			/*else if (flag_xor == 5 && flag_or == 15 && flag_and == 10)*/
				/*ft_strcat(output[i], "S ");*/
			/*else if (flag_xor == 10 && flag_or == 15 && flag_and == 5)*/
				/*ft_strcat(output[i], "S ");*/
			/*else if (flag_xor == 14 && flag_or == 15 && flag_and == 1)*/
				/*ft_strcat(output[i], "T ");*/
			/*else if (flag_xor == 37 && flag_or == 37 && flag_and == 0)*/
				/*ft_strcat(output[i], "T ");*/
			/*else if (flag_xor == 1 && flag_or == 1 && flag_and == 0)*/
				/*ft_strcat(output[i], "U ");*/
			/*else if (flag_xor == 1 && flag_or == 15 && flag_and == 14 && array[i].n_desc == 128)*/
				/*ft_strcat(output[i], "D ");*/
			/*else if (flag_xor == 23 && flag_or == 31 && flag_and == 8)*/
				/*ft_strcat(output[i], "D ");*/
			/*else if (flag_xor == 1 && flag_or == 15 && flag_and == 14 && array[i].n_desc == 0)*/
				/*ft_strcat(output[i], "b ");*/
			/*else if (flag_xor == 2 && flag_or == 14 && flag_and == 12)*/
				/*ft_strcat(output[i], "b ");*/
			/*else if (flag_xor == 18 && flag_or == 30 && flag_and == 12)*/
				/*ft_strcat(output[i], "b ");*/
			/*else if (flag_xor == 20 && flag_or == 30 && flag_and == 10)*/
				/*ft_strcat(output[i], "b ");*/
			/*else if (flag_xor == 8 && flag_or == 14 && flag_and == 6)*/
				/*ft_strcat(output[i], "d ");*/
			/*else if (flag_xor == 22 && flag_or == 30 && flag_and == 8)*/
				/*ft_strcat(output[i], "d ");*/
			/*else if (flag_xor == 16 && flag_or == 30 && flag_and == 14)*/
				/*ft_strcat(output[i], "d ");*/
			/*else if (flag_xor == 6 && flag_or == 30 && flag_and == 24)*/
				/*ft_strcat(output[i], "d ");*/
			/*else if (flag_xor == 21 && flag_or == 31 && flag_and == 10)*/
				/*ft_strcat(output[i], "d ");*/
			/*else if (flag_xor == 0 && flag_or == 14 && flag_and == 14)*/
				/*ft_strcat(output[i], "d ");*/
			/*else*/
				/*ft_strcat(output[i], "0 ");*/
		/*}*/
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
		/*ft_strcat(output[i], " ");*/
		/*ft_strcat(output[i], ft_itoa((int)array[i].n_value)); // debug flag NE PAS OUBLIER DE SUPPRIMER LE MALLOC + 2*/
		/*ft_strcat(output[i], " ");*/
		/*ft_strcat(output[i], ft_itoa((int)array[i].n_type & N_TYPE)); // debug flag NE PAS OUBLIER DE SUPPRIMER LE MALLOC + 2*/
		++i;
	}
	ft_strdel(&symbol);
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
	char					*segment_name[4096];

	i = 0;
	sym = NULL;
	header = (void *)ptr; // header pointe sur ptr (octet 0 du binaire)
	ncmds = header->ncmds; // ncmds contient le nombre de load_command
	lc = (void *)ptr + sizeof(struct mach_header_64); // lc pointe sur le debut de la zone des load commands (juste apres le header)
	ft_memset(segment_name, 0, 4096);
	if (header->filetype > 0xb)
	{
		printf(RED"Invalid file\n"END);
		return ;
	}
	while (i < ncmds) // on iter autant de fois qu'il y a de load commands
	{
		if (lc->cmd == LC_SYMTAB) // si la cmd est egal a LC_SYMTAB
			sym = (struct symtab_command *)lc;
		if (lc->cmd == LC_SEGMENT_64)
			get_seg_name_64((struct segment_command_64 *)lc, segment_name);
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
	}
	if (sym)
		print_output_64(sym, header, ptr, segment_name);
}
