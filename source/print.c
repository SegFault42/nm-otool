#include "nm.h"

static char	*symbol(uint8_t n_type, uint8_t n_sect)
{
	if ((n_type & n_sect) == 0x0)
		return ("U");
	else if ((n_type & n_sect) == 0x1)
		return ("T");
	else if ((n_type & n_sect) == 0x8)
		return ("d");
	else if ((n_type & n_sect) == 0x9 || (n_type & n_sect) == 0xc)
		return ("d");
	else if ((n_type & n_sect) == 0xb || (n_type & n_sect) == 0xd)
		return ("S");
	else if ((n_type & n_sect) == 0xa)
		return ("b");
	return ("0");
}

void	print_output(uint32_t nsyms, uint32_t symoff, uint32_t stroff, char *ptr)
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
		output[i] = (char *)ft_memalloc(sizeof(char) * 19 + ft_strlen(stringtable + array[i].n_un.n_strx) + 1 + 2);
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
		ft_strcat(output[i], symbol(array[i].n_type, array[i].n_sect));

		ft_strcat(output[i], stringtable + array[i].n_un.n_strx);
		++i;
	}
	ft_sort_double_array(output);
	ft_print_2d_tab(output);
	ft_2d_tab_free(output);
}
