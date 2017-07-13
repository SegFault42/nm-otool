#include "nm.h"

char *setup_unset(int argc, char *argv, t_setup *setup, char flag)
{
	if (flag == 's')
	{
		setup->fd = open(argv, O_RDONLY);
		if (setup->fd == -1)
		{
			ft_dprintf(STDERR_FILENO, "error: ./ft_otool: can't open file: %s (No such file or directory)\n", argv);
			return (NULL);
		}
		if (fstat(setup->fd, &setup->buf) == -1)
		{
			ft_dprintf(STDERR_FILENO, "fstat failure\n");
			return (NULL);
		}
		setup->ptr = mmap(0, (size_t)setup->buf.st_size, PROT_READ, MAP_PRIVATE, setup->fd, 0);
		if (setup->ptr == MAP_FAILED)
		{
			ft_dprintf(STDERR_FILENO, "mmap failure\n");
			return (NULL);
		}
	}
	else if (flag == 'u')
	{
		if (munmap(setup->ptr, (size_t)setup->buf.st_size) == -1)
		{
			ft_dprintf(STDERR_FILENO, "munap failure\n");
			return ((char *)-1);
		}
		ft_memset(setup, 0, sizeof(t_setup));
	}
	return (setup->ptr);
}
