/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/19 05:14:45 by rabougue          #+#    #+#             */
/*   Updated: 2017/08/08 02:51:56 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

static unsigned long	switch_hexa(unsigned long x)
{
	if (x <= 9)
		return (48 + x);
	if (x >= 10 && x <= 15)
	{
		x = x - 10;
		return ('A' + x);
	}
	return (0);
}

void					ft_print_hexa(unsigned long n)
{
	char			str[128];
	int				size;
	unsigned long	x;

	ft_memset(str, 0, 128);
	x = n;
	size = 0;
	while (x >= 16)
	{
		x /= 16;
		size++;
	}
	ft_strcat(str, "0x");
	str[size + 3] = '\0';
	while (size >= 0)
	{
		x = n % 16;
		str[size + 2] = switch_hexa(x);
		n /= 16;
		size--;
	}
	ft_putstr(str);
}
