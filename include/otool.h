/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/23 20:42:27 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/08 02:49:09 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OTOOL_H
# define OTOOL_H

# include "../libft/includes/libft.h"
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <mach-o/nlist.h>
# include <sys/stat.h>

typedef struct	s_setup
{
	struct stat	buf;
	char		*ptr;
	int			fd;
	char		pad[4];
}				t_setup;

char			*setup_unset(char *argv, t_setup *stp, char flag);
void			handle_64(char *ptr);
void			handle_32(char *ptr);
void			fat_handle(char *file, char *argv);

#endif
