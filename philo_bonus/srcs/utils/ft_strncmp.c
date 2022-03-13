/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 22:44:48 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 22:44:49 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	index;

	index = 0;
	if (n == 0)
		return (0);
	while ((s1[index] && s2[index]) && index < n)
	{
		if (s1[index] != s2[index] || index == n - 1)
			break ;
		index++;
	}
	return ((unsigned char)s1[index] - (unsigned char)s2[index]);
}
