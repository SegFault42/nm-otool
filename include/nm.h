/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/23 20:42:16 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/09 04:09:34 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include "../libft/includes/libft.h"
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <mach-o/nlist.h>
# include <sys/stat.h>

typedef struct s_setup	t_setup;

struct	s_setup
{
	struct stat	buf;
	char		*ptr;
	int			fd;
	char		pad[4];
};

char	*setup_unset(int argc, char *argv, t_setup *stp, char flag);
void	handle_64(char *ptr);
void	handle_64_revers(char *ptr);
void	handle_32(char *ptr);
char	symbol(size_t n_type, int value, char *buf);
void	fat_handle(char *file);

char	*get_symbol(struct symtab_command *sym, char *ptr, char **array);
void	get_seg_name_32(struct segment_command *segment, char **array);
void	swap(char **array, uint32_t nb);
void	delete_same_value_2(char **array, uint32_t nb);
void	delete_same_value(char **array, uint32_t nb);

void	print_output_64(struct symtab_command *sym, char *ptr, char **seg_name);
void	free_same(char **array, uint32_t nb);
void	swap_64(char **array, uint32_t nb);
char	*get_symbol_64(struct symtab_command *sym, char *ptr, char **array);
void	delete_same_value_64(char **array, uint32_t nb);
void	stock_symbol_64(char symbol, char **output);
void	stock_addr_64(uint64_t n_value, char **output);
void	sort_and_print_64(char **output, uint64_t nsyms);

#endif
