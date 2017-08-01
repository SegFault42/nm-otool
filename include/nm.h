/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/23 20:42:16 by rabougue          #+#    #+#             */
/*   Updated: 2017/07/23 20:42:17 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

#include "../libft/includes/libft.h"
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <sys/stat.h>

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
char	symbol(size_t n_type, int value, char *buf);

#endif
