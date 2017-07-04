#include "nm.h"

char *setup_unset(char *argv, t_setup *stp, char flag)
{
	if (flag == 's')
	{
		stp->fd = open(argv, O_RDONLY);
		if (stp->fd == -1)
		{
			ft_dprintf(STDERR_FILENO, "Open argv failure\n");
			return (NULL);
		}
		if (fstat(stp->fd, &stp->buf) == -1)
		{
			ft_dprintf(STDERR_FILENO, "fstat failure\n");
			return (NULL);
		}
		stp->ptr = mmap(0, (size_t)stp->buf.st_size, PROT_READ, MAP_PRIVATE, stp->fd, 0);
		if (stp->ptr == MAP_FAILED)
		{
			ft_dprintf(STDERR_FILENO, "mmap failure\n");
			return (NULL);
		}
	}
	else if (flag == 'u')
	{
		if (munmap(stp->ptr, (size_t)stp->buf.st_size) == -1)
		{
			ft_dprintf(STDERR_FILENO, "munap failure\n");
			return ((char *)-1);
		}
		ft_memset(stp, 0, sizeof(t_setup));
	}
	return (stp->ptr);
}
