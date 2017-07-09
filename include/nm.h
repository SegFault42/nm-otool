#ifndef NM_H
# define NM_H

#include "../libft/includes/libft.h"
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <sys/stat.h>

typedef struct		s_symtab
{
	char			*name;
	struct s_symtab	*next;
}					t_symtab;

typedef struct	s_setup
{
	struct stat	buf;
	char		*ptr;
	int			fd;
	char		pad[4];
}				t_setup;

char	*setup_unset(int argc, char *argv, t_setup *stp, char flag);
void	handle_64(char *ptr);
void	handle_64_revers(char *ptr);
void	handle_32(char *ptr);

#endif
