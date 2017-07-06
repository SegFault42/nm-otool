#include "nm.h"

static void	nm(char *ptr)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t *)(void *)ptr;
	if (magic_number == MH_MAGIC_64)
		handle_64(ptr);
	/*if (magic_number == MH_CIGAM_64)*/
		/*handle_64_revers(ptr);*/
	else
		ft_dprintf(2, RED"NOT SUPPORTED YET :(\n"END);
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
